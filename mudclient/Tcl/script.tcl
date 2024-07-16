###########################################
# script.tcl
# This file contains Tcl commands to emulate the MUPPETScript # commands used by the server. They basically act as stubs to # call the real server versions.
# Iain Ollerenshaw 10/2/98
# Version History:
# Date	Who Comments
# 10/2/98	IO	Created
###########################################
# proc to create an object
# Simply create the appropriate MUDScript string
# and send it. Returns result
proc create {type name args} {
    global clientId
    set theString ""
    set theResult ""
    set theString [concat $theString $clientId]
    set theString [concat $theString "CREATE"]
    set theString [concat $theString [string toupper $type]]
    set theString [concat $theString $name]
    foreach i $args {
        set theString [concat $theString $i]
    }

    # String is complete. Now pass to stringSend
    # This is turn passes the string to the C
    # command stringSendCmd which sends the string to
    # the server.
    
    set theResult [stringSendReceive $theString]
    if {$theResult == "-9"} {
        dialog .error {Script Error} \
          {The server is not responding. Please contact your systems administrator and ensure that the server is back up before clicking continue.}\
          {warning} 0 {Continue}
        return -1
    } else {
        return $theResult
    }
}

# Remove an object
# Simply create the appropriate MUDScript string
# and send it. Returns result
proc delete {name} {
    global clientId
    set theString ""
    set theResult ""
    set theString [concat $theString $clientId]
    set theString [concat $theString "DELETE"]
    set theString [concat $theString $name]
    
    # String is complete. Now pass to stringSend
    # This is turn passes the string to the C
    # command stringSendCmd which sends the string to
    # the server.
   set result [stringSendReceive $theString]
    if {$result == "-9"} {
        dialog .error {Script Error} \
          {The server is not responding. Please contact your systems administrator and ensure that the server is back up before clicking continue.}\
          {warning} 0 {Continue}
        return -1
    } else {
        # We now get the result string from stringReceive
        return $theResult
    }
}

# Edit basically performs the commit on an object
# the result is that the information contained in
# the local copy of the object is written to the
# database using the EDIT script command
proc edit {name args} {
    global clientId
    set theString ""
    set theResult ""
    set theString [concat $theString $clientId]
    set theString [concat $theString "EDIT"]
    set theString [concat $theString $name]
    foreach i $args {
        set theString [concat $theString $i]
    }
    # String is complete. Now pass to stringSend
    # This is turn passes the string to the C
    # command stringSendCmd which sends the string to
    # the server.
   set result [stringSendReceive $theString]
    if {$result == "-9"} {
        dialog .error {Script Error} \
          {The server is not responding. Please contact your systems administrator and ensure that the server is back up before clicking continue.}\
          {warning} 0 {Continue}
        return -1
    } else {
        # We now get the result string from stringReceive
        return $theResult
    }
}

# Draw. Given a name, returns the current state of the object
proc draw {name} {
    global clientId
    set theString ""
    set theResult ""
    set theString [concat $theString "DRAW"]
    set theString [concat $theString $name]

    # String is complete. Now pass to stringSend
    # This is turn passes the string to the C
    # command stringSendCmd which sends the string to
    # the server.
    set result [stringSendReceive $theString]
    if {$result =="-9"} {
        dialog .error {Script Error} \
          {The server is not responding. Please contact your systems administrator and ensure that the server is back up before clicking continue.}\
          {warning} 0 {Continue}
        return -1
    } else {
        # We now get the result string from stringReceive
        return $theResult
    }
}

# Update display from server
proc doRedraw {} {
   global objList clientId objData
   # Clear everything off screen that we don't own
   set numObj [llength $objList]
   for {set i 1} {$i < $numObj} {incr i 1} {
      set objName [lindex $objList $i]
      if {$objData($objName,7) != $clientId} {
         .test.canv delete $objName
      }
   }
   # Draw 'em all back from the server
   # except the ones we own
   drawAll notOwn
}

# This procedure is very importrant. It retrieves all object
# information from the server and creates the client
# internal state, including the display
# method states whether we want to update all objects
# or others apart from our own
proc drawAll {method} {
   global clientId
   global objData objList selectObj

   puts "TCL: In drawAll..."

   set theString ""
   set theResult ""
   set count 0

   # First we find the number of objects with DRAWALL
   set theString [concat $theString $clientId]
   set theString [concat $theString "DRAWALL"]
   set test [catch {set numObj [llength $objList]}]

   puts "TCL: Getting num objects..."
   set numObjects [stringSendReceive $theString]
   puts "TCL: Got num objects: numObjects..."

   if {$test != 0} {
       set numObj 0
   }
   for {set i 1} {$i < $numObj} {incr i 1} {
         set objName [lindex $objList $i]
         if {$objData($objName,7) != $clientId} {
           .test.canv delete $objName
         }
   }
   # We now do a loop and get object information # from each object using DRAWBYNUM
   while {$count <= [expr $numObjects - 1]} {
      set theString [concat $clientId "DRAWBYNUM" $count]
      set theResult [stringSendReceive $theString]
      # Extract the details
      set name [lindex $theResult 0]
      set type [lindex $theResult 1]
      set x1 [lindex $theResult 2]
      set y1 [lindex $theResult 3]
      set x2 [lindex $theResult 4]
      set y2 [lindex $theResult 5]
      set layer [lindex $theResult 6]
      set owner [lindex $theResult 7]
      set text [lindex $theResult 8]
         
      if {$owner != $clientId || $method == "all"} {
          # Setup local state
          set objData($name,1) $name;	# Name
          set objData($name,2) $x1;	# x1
          set objData($name,3) $y1;	# y1
          set objData($name,4) $x2;	# x2
          set objData($name,5) $y2;	# y2
          set objData($name,6) $layer; # layer
          set objData($name,7) $owner; # owner
          set objData($name,8) $type;	# type
          set objData($name,9) $text;	# text string
          
          lappend objList $name
          
          if {$owner == $clientId} {
              set colour yellow
          } elseif {$owner == 0} {
              set colour green
          } else {
              set colour red
          }
          # Draw it on the screen
          switch $type {
              1 {	# Draw a circle
                  drawCircle .test.canv $name $x1 $y1 $x2 $y2 $colour
              }
              2 { # Draw a box
                  drawBox .test.canv $name $x1 $y1 $x2 $y2  $colour
              }
              3 {	# Draw a line
                  drawLine .test.canv $name $x1 $y1 $x2 $y2 $colour
              }
              4 {	# Draw text
                  drawText .test.canv $name $x1 $y1 $text
              }
          }
      }
      incr count 1
    }
}

proc lock {name} {
   global clientId
   set theString ""
   set theResult ""
   set theString [concat $theString $clientId]
   set theString [concat $theString "LOCK"]
   set theString [concat $theString $name]

   # String is complete. Now pass to stringSend
   # This is turn passes the string to the C
   # command stringSendCmd which sends the string to
   # the server.
   set theResult [stringSendReceive $theString]
   return $theResult
}

# Unlocks an object
# Simply create the appropriate MUDScript string
# and send it. Returns result
proc unlock {name} {
   global clientId
   set theString ""
   set theResult ""
   set theString [concat $theString $clientId]
   set theString [concat $theString "UNLOCK"]
   set theString [concat $theString $name]
   
   # String is complete. Now pass to stringSend
   # This is turn passes the string to the C
   # command stringSendCmd which sends the string to
   # the server.
   set theResult [stringSendReceive $theString]
   return $theResult
}

# Force server to save objects
proc doSave	{} {
   global clientId
   set theString ""
   set theResult ""
   set theString [concat $theString $clientId]
   set theString [concat $theString "SAVE"]
   set theResult [stringSendReceive $theString]
   return $theResult
}
