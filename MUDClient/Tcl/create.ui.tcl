###########################################
# create.ui.tcl
# Iain Ollerenshaw 17/2/98
# Version History:
# Date	Who Comments
# 1/3/98	20	Created
###########################################
# interface generated by SpecTcl version 1.1 from /user.sh/io/mudcode/client/Tcl/create.ui
#	root	is the parent window for this user interface
# This procedure creates a widget for
# getting information for creating objects
proc create_ui {root args} {
# this treats "." as a special case
    if {$root == "."} {
        set base ""
    } else {
        set base $root
    }
    
    frame $base.frame#1
    frame $base.frame#2 -borderwidth 1
    
    label $base.label#2 -text Type
    
    radiobutton $base.ovalRadio \
    -text Oval \
    -variable example_radiobutton
    
    radiobutton $base.boxRadio \
    -text Box \
    -variable boxRad
    
    radiobutton $base.lineRad \
    -text Line \
    -variable lineRad
    
    radiobutton $base.textRad \
    -text Text \
    -variable textRad
    
    entry $base.entry#1 \
    -textvariable entry
    
    label $base.label#3 \
    -text Name
    
    entry $base.entry#2 \
    -textvariable entry
    
    label $base.label#5 \
    -text Position
    
    entry $base.tlxEntry \
    -textvariable tlx \
    -width 3
    
    entry $base.tlyEntry \
    -textvariable tly \
    -width 3
    
    entry $base.brxEntry \
    -textvariable brx \
    -width 3
    
    entry $base.bryEntry \
    -textvariable bry \
    -width 3
    
    label $base.label#6 \
    -text Layer
    
    entry $base.layerEntry \
    -textvariable layer \
    -width 3
    
    button $base.createButt \
    -text Create
    
    button $base.cancelButt \
    -text Cancel
    
    # Geometry Management
    grid $base.frame#1 -in $root -row 4 -column 2 -columnspan 2
    grid $base.frame#2 -in $root -row 3 -column 1 -columnspan 4
    grid $base.label#2 -in $root -row 1 -column 1 -sticky nesw
    grid $base.ovalRadio -in $root	-row 1 -column 2 -sticky nesw
    grid $base.boxRadio -in $root -row 1 -column 3 -sticky nesw
    grid $base.lineRad -in $root -row 2 -column 2 -sticky nesw
    grid $base.textRad -in $root -row 2 -column 3 -sticky nesw
    grid $base.entry#1 -in $root -row 2 -column 4
    grid $base.label#3 -in $root -row 4 -column 1 -sticky nesw
    grid $base.entry#2 -in $base.frame#1	-row 1 -column 1
    grid $base.label#5 -in $root -row 5 -column 1 -sticky nesw
    grid $base.tlxEntry -in $root -row 5 -column 2 -sticky nesw
    grid $base.tlyEntry -in $root -row 5 -column 3 -sticky nesw
    grid $base.brxEntry -in $root -row 6 -column 2 -sticky nesw
    grid $base.bryEntry -in $root -row 6 -column 3 -sticky nesw
    grid $base.label#6 -in $root -row 7 -column 1 -sticky nesw
    grid $base.layerEntry -in $root	-row 7 -column 2 -sticky nesw
    grid $base.createButt -in $root	-row 9 -column 1 -sticky nesw
    grid $base.cancelButt -in $root	-row 9 -column 3 -sticky nesw
    
    # Resize behavior management
    grid rowconfigure $root 1 -weight 0 -minsize 30
    grid rowconfigure $root 2 -weight 0 -minsize 30
    grid rowconfigure $root 3 -weight 0 -minsize 30
    grid rowconfigure $root 4 -weight 0 -minsize 30
    grid rowconfigure $root 5 -weight 0 -minsize 30
    grid rowconfigure $root 6 -weight 0 -minsize 30
    grid rowconfigure $root 7 -weight 0 -minsize 30
    grid rowconfigure $root 8 -weight 0 -minsize 30
    grid rowconfigure $root 9 -weight 0 -minsize 30
    grid columnconfigure $root 1 -weight 0 -minsize 30
    grid columnconfigure $root 2 -weight 0 -minsize 2
    grid columnconfigure $root 3 -weight 0 -minsize 54
    grid columnconfigure $root 4 -weight 0 -minsize 30
    
    grid columnconfigure $base.frame#1 1 -weight 0 -minsize 128
    
    # additional interface code
    # end additional interface code
    
}
# Allow interface to be run "stand-alone" for testing
catch {
    if [info exists embed_args]
    {
        # we are running in the plugin
        create_ui .
    }
    else
    {
        # we are running in stand-alone mode
        if {$argv0 == [info script]}
        {
            wm title . "Testing create_ui"
            create_ui .
        }
    }
}

create_ui .