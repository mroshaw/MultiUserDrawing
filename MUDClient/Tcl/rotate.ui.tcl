###########################################
# rotate.ui.tcl
# Display the rotate dialog box
# Iain Ollerenshaw 1/3/98
# Version History:
# Date	Who Comments
# 1/3/98	IO	Created
###########################################
#	interface generated by SpecTcl version 1.1 from Oli's HD:SHProject:MUD Code:Client:Tcl:rotate.ui
#	root	is the parent window for this user interface
# Create the print dialog box
proc rotate_ui {root args} {

    global selectedObj

    # this treats "." as a special case

    if {$root == "."} {
        set base ""
    } else {
        set base $root
    }

    label $base.label#1 -text Rotate
    label $base.objLabel -text label
    label $base.label#3 -text By
    entry $base.rotateEntry -textvariable rotate -width 3
    button $base.okayButt -text Okay -command {rotate $selectObj $rotate; destroy .rotate}
    button $base.cancelButton -text Cancel -command {destroy .rotate}
                                                     
    # Geometry management
    grid $base.label#1 -in $root -row 1 -column 1 -sticky nesw
    grid $base.objLabel -in $root -row 1 -column 2 -sticky nesw
    grid $base.label#3 -in $root -row 2 -column 1 -sticky nesw
    grid $base.rotateEntry -in $root	-row 2 -column 2 -sticky nesw
    grid $base.okayButt -in $root -row 3 -column 1 -sticky nesw
    grid $base.cancelButton -in $root -row 3 -column 2 -sticky nesw
    
    # Resize behavior management
    grid rowconfigure $root 1 -weight 0 -minsize 30
    grid rowconfigure $root 2 -weight 0 -minsize 30
    grid rowconfigure $root 3 -weight 0 -minsize 30
    grid columnconfigure $root 1 -weight 0 -minsize 30
    grid columnconfigure $root 2 -weight 0 -minsize 30
    
    # additional interface code
    # end additional interface code
}
# Allow interface to be run "stand-alone" for testing
catch {
    if [info exists embed_args] {
        # we are running in the plugin
        rotate_ui .
    } else {
        # we are running in stand-alone mode
        if {$argv0 == [info script]} {
            wm title . "Testing rotate_ui"
            rotate_ui .
        }
    }
}