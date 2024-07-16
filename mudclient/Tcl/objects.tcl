###########################################
# objects.tcl
# Routines for creating and manipulating objects
# in local and remote systems. Differs from
# draw and script files as this links the two
# Iain Ollerenshaw 11/2/98
# Version History:
# Date	Who Comments
# 11/2/98	IO	Created
###########################################
# This proc creates a circle at the specified co-ords
# and draws it on screen and creates it in the database
proc makeCircle {canpath x1 y1 x2 y2} {
    global clientId
    
    # Local (to App) store of all objects
    global objData objList
    
    set name [txtDiag .m {Creating..} {Enter name for circle:} {} 0 Okay Cancel]
    
    if {$name != ""} {
        # Create circle in database
        set result [create circle $name $x1 $y1 $x2 $y2]
        
        # If successful, do the rest
        if {$result == 1} {
            # Create the circle in local system
            set objData($name,1) $name;	# Name
            set objData($name,2) $x1;	# x1
            set objData($name,3) $y1;	# y1
            set objData($name,4) $x2;	# x2
            set objData($name,5) $y2;	# y2
            set objData($name,6) 0;	# layer
            set objData($name,7) $clientId;	# owner
            set objData($name,8) 1;	# Type
            lappend objList $name
            
            # Draw it on the screen!
            drawCircle $canpath $name [transX $x1] [transY $y1] [transX $x2] [transY $y2] yellow
            
            # Just to be safe, we should really force the
            # server to save contents to disk
            doSave
            # Groovy!
        } else {
                # An error has occured!
                doScriptError $result
        }
    }
}

# This proc creates a box at the specified co-ords
# and draws it on screen and creates it in the database
proc makeBox {canpath x1 y1 x2 y2} {
    
    global clientId
    # Local (to App) store of all objects
    global objData objList
    
    set name [txtDiag .m {Creating box..} {Enter name for box:} {} 0 Okay Cancel]
    if {$name != ""} {
        # Create box in database
        set result [create box $name $x1 $y1 $x2 $y2]
        # If successful, do the rest!
        if {$result == 1} {
            # Create the box in local system
            set objData($name,1) $name;	# Name
            set objData($name,2) $x1;	# x1
            set objData($name,3) $y1;	# y1
            set objData($name,4) $x2;	# x2
            set objData($name,5) $y2;	# y2
            set objData($name,6) 0;	# layer
            set objData($name,7) $clientId;	# owner
            set objData($name,8) 2; # Type
            lappend objList $name
            # Draw it on the screen!
            drawBox $canpath $name [transX $x1] [transY $y1] [transX $x2] [transY $y2] yellow
            # Just to be safe, we should really force the
            # server to save contents to disk
            doSave
            # Groovy!
        } else {
            # An error has occured!
            doScriptError $result
        }
    }
}

# This proc creates a line at the specified co-ords
# and draws it on screen and creates it in the database
proc makeLine {canpath x1 y1 x2 y2} {
    global clientId
    # Local (to App) store of all objects
    global objData objList
    
    set name [txtDiag .m {Creating..} {Enter name for line:} {} 0 Okay Cancel]
    if {$name != ""} {
        # Create line in database
        set result [create line $name $x1 $y1 $x2 $y2]
        # If successful, do the rest!
        if {$result == 1} {
            # Create the line in local system
            set objData($name,1) $name;	# Name
            set objData($name,2) $x1;	# x1
            set objData($name,3) $y1;	# y1
            set objData($name,4) $x2;	# x2
            set objData($name,5) $y2;	# y2
            set objData($name,6) 0;	# layer
            set objData($name,7) $clientId;	# owner set objData($name,8) 3; # Type
            lappend objList $name
            # Draw it on the screen!
            drawLine $canpath $name [transX $x1] [transY $y1] [transX $x2] [transY $y2] yellow
            # Just to be safe, we should really force the
            # server to save contents to disk
            doSave
            # Groovy!
        } else {
            # An error has occured!
            doScriptError $result
        }
    }
}
# This proc creates text at the specified co-ords
# and draws it on screen and creates it in the database
proc makeText {canpath x1 y1} {

    global clientId
    # Local (to App) store of all objects
    global objData objList
    
    set name [txtDiag .m {Creating..} {Enter name for text:} {} 0 Okay Cancel]
    if {$name != ""} {
        # Get the text
        set text [txtDiag .m {Text..} {Enter text:} {} 0 Okay Cancel]
        if {$text != ""} {
            # Create line in database
            set result [create text $name $x1 $y1 0 0 0 $text]
            # If successful, do the rest!
            if {$result == 1} {
                # Create the line in local system
                set objData($name,1) $name;	# Name
                set objData($name,2) $x1;	# x1
                set objData($name,3) $y1;	# y1
                set objData($name,4) 0;	# x2
                set objData($name,5) 0;	# y2
                set objData($name,6) 0;	# layer
                set objData($name,7) $clientId;	# owner
                set objData($name,8) 4;	# Type
                set objData($name,9) $text;	# Text string
                lappend objList $name
                # Draw it on the screen!
                drawText $canpath $name [transX $x1] [transY $y1] $text
                # Just to be safe, we should really force the
                # Groovy!
            } else {
                # An error has occured!
                doScriptError $result
            }
        }
    }
}


# Performs an edit on the object by checking the owner
# and displaying an edit box. Edit are actually done
# from the editBox_ui procedure.
proc editObject {theName} {
    global objData newName tlx tly brx bry newLayer clientId
    
    # Check that user owns object!
    if {$objData($theName,7) != $clientId} {
        dialog .w {Error} {You don't own this object!} {error} 0  {Oops}
    } else {
        toplevel .edit
        wm geometry .edit =+440+430
        wm title .edit {Editing object...}
        editBox_ui .edit
        set newName $objData($theName,1)
        set tlx $objData($theName,2)
        set tly $objData($theName,3)
        set brx $objData($theName,4)
        set bry $objData($theName,5)
        set newLayer $objData($theName,6)
        .edit.ownerLabel configure -text $objData($theName,7)
    }
}

# Perfrom the edit in local state
proc doEdit {theName x1 y1 x2 y2 newlayer} {
    global objData
    
    # Delete original
    .test.canv delete $theName
    
    # Setup local state
    # set objData($theName,1) $name; # Name
    set objData($theName,2) $x1; # x1
    set objData($theName,3) $y1; # y1
    set objData($theName,4) $x2; # x2
    set objData($theName,5) $y2; # y2
    set objData($theName,6) $newlayer;# layer
    set type $objData($theName,8)
    puts $type
    # Redraw on screen!
    switch $type {
        1 {	# Draw a circle
            drawCircle .test.canv $theName $x1 $y1 $x2 $y2 yellow
        }
        2 { # Draw a box
            drawBox .test.canv $theName $x1 $y1 $x2 $y2 yellow
        }
        3  { # Draw a line
            drawLine .test.canv $theName $x1 $y1 $x2 $y2 yellow
        }
        4 {	# Draw text
            drawText .test.canv $theName $x1 $y1 $objData($theName,9)
        }
    }
}

# Perform the locking of an object
proc doLock {objName} {
    global button objData clientId
    
    set msg [concat "Are you sure you want to lock object:" $objName]
    dialog .m {Locking...} $msg {questhead} 0 Yes No
             
    if {$button == 0} {
        set result [lock $objName]
        if {$result !=1} {
            # Unsuccessful!
            doScriptError $result
        } else {
            # Okay. Update internal state
            set objData($objName,7) $clientId
            # Update the colour!
            .test.canv itemconfigure $objName -fill yellow
        }
    }
}
# Commit changes. User is asked if he / she want to relinquish lock
# on the object after the database has been updated.
proc doCommit {objName} {
    global objData button
    set msg [concat "Are you sure you want to commit changes to object:" $objName]
    dialog .m {Committing...} $msg {questhead} 0 Yes No
    if {$button == 0} {
        edit $objName $objData($objName,1) $objData($objName,2) $objData($objName,3) $objData($objName,4) $objData($objName,5) $objData($objName,6)
        set msg [concat "Do you want to relinquish lock of:" $objName]
        dialog .m {Committing...} $msg {questhead} 1 Yes No
        if {$button == 0} {
            set result [unlock $objName]
            if {$result != 1} {
                doScriptError $result
            } else {
                set objData($objName,7) 0
                # Do an explicit server save here
                doSave
                # Update the colour!
                .test.canv itemconfigure $objName -fill green
            }
        }
     }
    # Do an explicit server save here
    doSave
}

# Delete an object
proc doDelete {objName} {
    global objData button objList
    set msg [concat "Are you sure you want to delete object:" $objName]
    dialog .d {Committing...} $msg {questhead} 0 Yes No
    if {$button == 0} {
        # Delete from database
        set result [delete $objName]
        if {$result != 1} {
            # Unsuccessful!
            doScriptError $result
        } else {
            # Delete from local store
            # Find object in local list
            set element [lsearch $objList $objName]
            # Remove it from local list
            lreplace $objList $element $element
            # Delete from picture
            .test.canv delete $objName

            # Just to be safe, we should really force the
            # server to save contents to disk
            doSave
        }
    }
}

# Do the actual object update on local and remote store
proc doObjUpdate {objName} {
    global objData clientId
    
    # Delete the original
    .test.canv delete $objName
    
    # Redraw from server
    set theResult [draw $objName]
    
    # Extract the details
    set name [lindex $theResult 0]
    set type [lindex $theResult 1]
    set x1 [lindex $theResult 2]
    set y1 [lindex $theResult 3]
    set x2 [lindex $theResult 4]
    set y2 [lindex $theResult 5]
    set layer [lindex $theResult 6]
    set owner [lindex $theResult 7]
    
    # Setup local state
    set objData($name,1) $name;	# Name
    set objData($name,2) $x1;	# x1
    set objData($name,3) $y1;	# y1
    set objData($name,4) $x2;	# x2
    set objData($name,5) $y2;	# y2
    set objData($name,6) 0;	# layer
    set objData($name,7) $owner; # owner
    set objData($name,8) $type;	# Type
    
    # Draw it on the screen
    if {$owner == $clientId} {
        set colour yellow
    } elseif {$owner == 0} {
        set colour green
    } else {
        set colour red
    }
    switch $type {
        1 {	# Draw a circle
            drawCircle .test.canv $name $x1 $y1 $x2 $y2 $colour
        }
        2 {	# Draw a box
            drawBox .test.canv $name $x1 $y1 $x2 $y2 $colour
        }
        3 {	# Draw a circle
            drawLine .test.canv $name $x1 $y1 $x2 $y2 $colour
        }
        4 {	# Draw text
            drawText .test.canv $theName $x1 $y1 $objData($theName,9)
        }
    }
}
