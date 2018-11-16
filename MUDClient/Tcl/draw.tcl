###########################################
# draw.tcl
# Routines for manipulating objects on
# screen.
# Iain Ollerenshaw 17/2/98
# Version History:
# Date  Who Comments
# 12/1/97   I0  Created
# 20/2/98   IO  Added object box
###########################################
#   Update the object box with
#   relevant information
proc updateBox {theName} {
    global objData selectObj
    
    .object.nameLabel configure -text $theName
    .object.ownerLabel configure -text $objData($theName,7)
    .object.layerLabel configure -text $objData($theName,6)
    .object.tlx1abel configure -text $objData($theName,2)
    .object.tly1abel configure -text $objData($theName,3)
    .object.brx1abel configure -text $objData($theName,4)
    .object.bry1abel configure -text $objData($theName,5)
    
    set selectObj $theName
}

#   Do things depending on what tool we have selected.
proc doTool {theName} {
    global currObject
    if {$currObject == 7} {
        #   We want to try to lock the object!
        doLock $theName
    } elseif {$currObject == 8} {
        #   We want to try to commit changes
        doCommit $theName
    } elseif {$currObject == 9} {
        #   We want to try to delete object
        doDelete $theName
    }
}

# Draw a circle on the canvas and events to
# mouse events
proc drawCircle {canpath tag x1 y1 x2 y2 colour} {
    global objData currX currY clientId
    $canpath create oval $x1 $y1 $x2 $y2 -fill $colour -tags $tag
    $canpath bind $tag <Button-1> {
        set tagname [%W gettags [%W find withtag current]]
        set theName [lindex $tagname 0]
        # Fill in object box
        updateBox $theName
        set currX %x
        set currY %y
        doTool $theName
    }
    $canpath bind $tag <Double-1> {
        set tagname [%W gettags [%W find withtag current]]
        set theName [lindex $tagname 0]
        editObject $theName
    }
    $canpath bind $tag <B1-Motion> {
        set tagname [%W gettags [%W find withtag current]]
        set theName [lindex $tagname 0]
        moveObject $theName [expr %x - $currX] [expr %y - $currY]
        
        # Update internal state
        set currX %x
        set currY %y
    }
}

# Moves an object by a specified distance
proc moveObject {object xDist yDist} {
    global currObject objData clientId
    
    # Move the object if in select mode and if user owns object
    if {$currObject == 1 && $objData($object,7) == $clientId} {
        # Draw in new position
        .test.canv move $object $xDist $yDist
        # Update local state
        incr objData($object,2) $xDist
        incr objData($object,3) $yDist
        incr objData($object,4) $xDist
        incr objData($object,5) $yDist
    }
}

# Draw a box at the specified location and bind
proc drawBox {canpath tag x1 y1 x2 y2 colour} {
    global objData currX currY
    $canpath create rectangle $x1 $y1 $x2 $y2 -fill $colour -tags $tag
    $canpath bind $tag <Button-1> {

        set tagname [%W gettags [%W find withtag current]] 
        set theName [lindex $tagname 0]
        updateBox $theName
        set currX %x
        set currY %y
        doTool $theName
    }
    
    $canpath bind $tag <Double-1> {
        set tagname [%W gettags [%W find withtag current]]
        set theName [lindex $tagname 0]
        editObject $theName
    }
    $canpath bind $tag <B1-Motion> {
        set tagname [%W gettags [%W find withtag current]]
        set theName [lindex $tagname 0]
        moveObject $theName [expr %x - $currX] [expr %y - $currY]

        # Update internal state
        set currX %x
        set currY %y
    }
}

# Draw a line at the specified location and bind
# commands to mouse events
proc drawLine {canpath tag x1 y1 x2 y2 colour} {
    global objData currX currY
    $canpath create line $x1 $y1 $x2 $y2 -fill $colour -width 5 -tags $tag
    $canpath bind $tag <Button-1> {

        set tagname [%W gettags [%W find withtag current]]
        set theName [lindex $tagname 0]
        updateBox $theName
        set currX %x
        set currY %y
        doTool $theName
    }
    $canpath bind $tag <Double-1> {
        set tagname [%W gettags [%W find withtag current]]
        set theName [lindex $tagname 0]
        editObject $theName
    }
    $canpath bind $tag <B1-Motion> {
        set tagname [%W gettags [%W find withtag current]]
        set theName [lindex $tagname 0]
        moveObject $theName [expr %x - $currX] [expr %y - $currY]

        # Update internal state
        set currX %x
        set currY %y
    }
}

# Draw some text at the specified location and bind
# commands to mouse events
proc drawText {canpath tag x1 y1 text} {
    global objData
    $canpath create text $x1 $y1 -text $text -tags $tag
    $canpath bind $tag <Button-1> {
        set tagname [%W gettags [%W find withtag current]]
        set theName [lindex $tagname 0]
        updateBox $theName
        set currX %x
        set currY %y
        doTool $theName
    }
    $canpath bind $tag <Double-1> {
        set tagname [%W gettags [%W find withtag current]]
        set theName [lindex $tagname 0]
        editObject $theName
    }
    $canpath bind $tag <B1-Motion> {
        set tagname [%W gettags [%W find withtag current]]
        set theName [lindex $tagname 0]

        moveObject $theName [expr %x - $currX] [expr %y - $currY]

        set currX %x
        set currY %y
    }
}

# Process a canvas click
proc doClick {canpath x y} {
    global currX
    global currY

    # What type of object are we drawing?
    global currObject
    if {$currObject == 1} {
        # Select
    } elseif {$currObject == 5} {
        # Text selected
        makeText $canpath $x $y
    }
    # set co-ords to be click
    set currX $x
    set currY $y
}

# Process a canvas button-up event
proc doReleaseClick {canpath x y} {
    global currX
    global currY
    
    # What type of object are we drawing?
    global currObject
    
    if {$currObject == 3} {
        #	Draw circle
        makeCircle $canpath $currX $currY $x $y
    } elseif {$currObject == 2} {
        #	Draw box
        makeBox $canpath $currX $currY $x $y
    } elseif {$currObject == 4} {
        #	Draw line
        makeLine $canpath $currX $currY $x $y
    }
}

# Process a drag event on the canvas
proc doCanvDrag {canvPath x y} {
 
    global currObject currX currY
    # Do some glimmering object thingies to make # things look cute.
    if {$currObject == 3} {
        # Draw circle
        $canvPath create oval [transX $currX] [transY $currY] [transX $x] [transY $y] -tags _temp
        after 200 {
            .test.canv delete _temp
        }
    } elseif {$currObject == 2} {
        # Draw box
        $canvPath create rectangle [transX $currX] [transY $currY] [transX $x] [transY $y] -tags _temp
        after 200 {
            .test.canv delete _temp
        }
    } elseif {$currObject == 4} {
        # Draw line
        $canvPath create line [transX $currX] [transY $currY] [transX $x] [transY $y] -tags _temp
        after 200 {
            .test.canv delete _temp
        }
    }
}

# Create a test window. Basically, this creates our canvas in a window
# May end up putting in scroll bars if I have time (IC - 15/2/98)
# Puts scrollbars in (I0 - 26/2/98)
# Made scrollbars work! (I0 - 11/3/98)
 proc testWin {} {
    # Selected toolbar button
    global currType
    toplevel .test
    wm geometry .test =733x689+130+214
    wm title .test {Multi User Drawing 1}
    .test configure -cursor {cross}
    canvas_ui .test
}

# Get information from user on rotate. ie. degrees
proc doRotate {objName} {
    catch {
        destroy .rotate
    }
    toplevel .rotate
    rotate_ui .rotate
    wm title .rotate {Rotate}
    .rotate.objLabel configure -text $objName
}

# Check user owns object and run scale window
proc doScale {objName} {
    global objData clientId
    if {$objData($objName,7) != $clientId} {
        dialog .w {Error} {You don't own this object!} {error} 0 {Oops}
    } else {
        catch {
            destroy .scale
        }
        toplevel .scale
        scale_ui .scale
        wm title .scale {Scale}
        .scale.objLabel configure -text $objName
    }
}

# Do the actual scaling and call edit to redraw
# the object
proc scale {objName scalex scaley} {
    global objData
    
    # Get old values
    set x1 $objData($objName,2)
    set y1 $objData($objName,3)
    set x2 $objData($objName,4)
    set y2 $objData($objName,5)
    set layer $objData($objName,6)
    
    # Work out the new figures..
    set difx [expr $x2 - $x1]
    set dify [expr $y2 - $y1]
    # Get a proper scaling fraction
    
    set scalex [expr $scalex / 100.0]
    set scaley [expr $scaley / 100.0]

    # Do the scaling
    if {$scalex < 1}  {
        set newx2 [expr [expr $x2 * $scalex] - $difx]
    } else {
        set newx2 [expr [expr $x2 * $scalex] + $difx]
    }
    
    if {$scaley < 1} {
        set newy2 [expr [expr $y2 * $scaley] - $dify]
    } else {
        set newy2 [expr [expr $y2 * $scaley] + $dify]
    }

    # Convert from float to int
    set newx2 [expr round($newx2)]
    set newy2 [expr round($newy2)]
    doEdit $objName $x1 $y1 $newx2 $newy2 $layer
}

# Translate an x screen coordinate into a canvas one
proc transX {x} {
    return [.test.canv canvasx $x]
}
# Translate an y screen coordinate into a canvas one
proc transY {y} {
    return [.test.canv canvasy $y]
}
