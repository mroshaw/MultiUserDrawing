###########################################
# about.ui.tcl
# Procedure for creating the about box
# Iain Ollerenshaw 17/2/98
# Version History:
# Date	Who Comments
# 17/2/98	IO	Created
###########################################
# interface generated by SpecTcl version 1.1 from Oli's HD:SHProject:mudcode:client:Tcl:about.ui
#	root	is the parent window for this user interface

proc about_ui {root args} {
    # this treats "." as a special case

    if {$root == "."} {
        set base ""
    } else {
        set base $root
    }
    image create photo pic -file [file join "C://MUD//MUDClient//Tcl//images//splash2.gif"]
    
    label $base.picLabel \
    -text label \
    -image pic
    
    label $base.infoLabel \
    -text {lain Ollerenshaw 1998}

    puts $root
    
    button $base.okayButton \
    -command { destroy .about} \
    -default active \
    -text Okay

    # Geometry management
    grid $base.picLabel -in $root -row 1 -column 1
    grid $base.infoLabel -in $root	-row 2 -column 1
    grid $base.okayButton -in $root	-row 3 -column 1

    # Resize behavior management
    grid rowconfigure $root 1 -weight 1 -minsize 30
    grid rowconfigure $root 2 -weight 0 -minsize 30
    grid rowconfigure $root 3 -weight 0 -minsize 30
    grid columnconfigure $root 1 -weight 0 -minsize 30

    # additional interface code

    # end additional interface code
}
# Allow interface to be run "stand-alone" for testing
catch  {
    if [info exists embed_args] {
        # we are running in the plugin
        about_ui .
    } else {
        # we are running in stand-alone mode
        if {$argv0 == [info script]} {
            wm title . "Testing about_ui"
            about_ui .
        }
    }
}
