########################################### # splash.tcl
# Do the splash screen.
# Iain Ollerenshaw 5/3/98
# Version History:
# Date	Who Comments
# 5/3/98	IO	Created
###########################################
# Display the splash screen. Relies on other
# code to destroy it.
proc doSplash {} {
    toplevel .splash -class Dialog
    wm geometry .splash =+440+300
    wm title .splash {MUD}
    image create photo pic -file [file join "C://MUD//MUDClient//Tcl//images//splash2.gif"]
    label .splash.picture -image pic
    pack .splash.picture
    catch {grab set -global .splash}
}
