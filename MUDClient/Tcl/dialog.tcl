###########################################
# dialog.tcl
# Generic routines for creating dialogs,
# about and quitting
# Iain Ollerenshaw 17/2/98
# Version History:
# Date	Who Comments
# 3/12/97	IO	Created
###########################################
# Creates a standard dialog box with title, text,
# bitmap and buttons specified

proc dialog {w title text bitmap default args} {
    global button
    
    catch {destroy $w}
    toplevel $w -class Dialog

    wm geometry $w =+440+430
    wm title $w $title
    wm iconname $w Dialog

    frame $w.top -relief raised -bd 1
    pack $w.top -side top -fill both
    frame $w.bot -relief raised -bd 1
    pack $w.bot -side bottom -fill both

    message $w.top.msg -width 3i -text $text
    pack $w.top.msg -side right -expand 1 -fill both -padx 3m -pady 3m

    if {$bitmap != ""} {
        label $w.top.bitmap -bitmap $bitmap
        pack $w.top.bitmap -side left -padx 3m -pady 3m
    }

    set i 0
    foreach but $args {
        button $w.bot.button$i -text $but -command\
            "set button $i"
        if {$i == $default} {
            frame $w.bot.default -relief sunken -bd 1
            raise $w.bot.button$i
            pack $w.bot.default -side left -expand 1\
            -padx 3m -pady 2m
            pack $w.bot.button$i -in $w.bot.default\
            -side left -padx 2m -pady 2m\
            -ipadx 2m -ipady 1m
        } else {
            pack $w.bot.button$i -side left -expand 1\
            -padx 3m -pady 3m -ipadx 2m -ipady 1m
        }
        incr i
    }
    
    if {$default >=0} {
        bind $w <Return> "$w.bot.button$default flash; \
        set button $default"
    }
    
    set oldFocus [focus]
    tkwait visibility $w
    grab set $w
    focus $w

    tkwait variable button
    catch { destroy $w }
    focus $oldFocus
    return $button
}

# Creates a dialog box which also contains a text field
# the contents of which is returned
proc txtDiag {w title text bitmap default args} {
    global button
    global dialogResult
    set dialogResult ""

    catch { destroy $w }
    
    toplevel $w -class Dialog
    wm geometry $w =+440+430
    
    # Delay to allow window to appear
    while { [catch {wm title $w $title}] !=0 } { }

    wm iconname $w Dialog
    frame $w.top -relief raised -bd 1
    pack $w.top -side top -fill both
    frame $w.mid -relief raised -bd 1
    pack $w.mid -side top -fill both
    frame $w.bot -relief raised -bd 1
    pack $w.bot -side bottom -fill both

    message $w.top.msg -width 3i -text $text
    pack $w.top.msg -side right -expand 1 -fill both -padx 3m -pady 3m

    if {$bitmap != ""} {
        label $w.top.bitmap -bitmap $bitmap
        pack $w.top.bitmap -side left -padx 3m -pady 3m
    }
    entry $w.mid.entry -relief sunken -bd 2 \
    -textvariable dialogResult \
    -width 20

    pack $w.mid.entry -side left -fill both

    set i 0
    foreach but $args {
        button $w.bot.button$i -text $but -command\
            "set button $i"
        if {$i == $default} {
            frame $w.bot.default -relief sunken -bd 1
            raise $w.bot.button$i
            pack $w.bot.default -side left -expand 1\
            -padx 3m -pady 2m
            pack $w.bot.button$i -in $w.bot.default\
            -side left -padx 2m -pady 2m\
            -ipadx 2m -ipady 1m
        } else {
            pack $w.bot.button$i -side left -expand 1\
            -padx 3m -pady 3m -ipadx 2m -ipady 1m
        }
        incr i
    }
    
    if {$default >=0} {
        bind $w <Return> "$w.bot.button$default flash;
        set button $default"
    }
    
    set oldFocus [focus]
    tkwait visibility $w
    catch { grab set $w }
    focus $w
    
    tkwait variable button
    destroy $w
    focus $oldFocus
    
    if {$button==1} {
        return ""
    } else {
        if {[string length $dialogResult] > 15} {
            dialog .w2 Oops {Must be less than 15 chars} {} 0 Okay
            return ""
        } else {
            return $dialogResult
        }
    }
}
        
# Display a help box with help
proc doHelp {} {
    dialog .w Help... {No help yet} {} 0 Alright!
}

# Confirm if user want to quit
proc doQuit {} {
    set really [dialog .w Quit... {Are you sure?} {questhead} 1 Yup! Nope!]
    if {$really == 0} {
        set really [dialog .w Quit... {Call server save?} {questhead} 0 Aye! Na!]
        if {$really == 0} {
            doSave
        }
        # Stop redrawing from server
        cancelUpdate
        # Destroy all windows and terminate destroy .
        destroy .
    }
}

# Standard dialog to inform user of
# un-implemented features
proc notDone {} {
    dialog .w Ooops! {Sorry. Not yet implemented!} {} 0 {I'll wait}
}

# Display the about box
proc doAbout {} {
    toplevel .about
    about_ui .about
}