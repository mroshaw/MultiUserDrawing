###########################################
# getPrefs.tcl
# Routines for handling preferences in MUD
# We get some prefs from file:
# serverName, printerName, updateRate
# And some from internal calls:
# clientId
# Iain Ollerenshaw 17/2/98
# Version History:
# Date	Who Comments
# 4/3/98	I0	Created
###########################################
# Get the preferences on startup. If the
# preferences file does not exist then it is
# created and the user is asked to fill them in

proc getPrefs {args} {
    global clientId serverName portNum printerName updateRate
    # Get userlD from getUID
    set clientId [getID]
    set res [catch {set f [open "./MUDClient.prefs" r]}]
    # No prefs file here!
    if {$res != 0} {
        dialog .d {getPrefs} {Prefs file not found! Making a new one!} {warning} 0 {Okay!}
        doNewPrefs
    } else {
        #	Get serverId
        gets $f line
        set serverName [lindex $line 1]
        #	Get port number
        gets $f line
        set portNum [lindex $line 1]
        #	Okay! Close the file!
        #	Get printer name
        gets $f line
        set printerName [lindex $line 1]
        #	Get printer name
        gets $f line
        set updateRate [lindex $line 1]
        # Okay! Close the file!
        close $f
    }
}
# Get new prefs from a dialog box
proc doNewPrefs	{} {
    global prefsDone
    catch {destroy .prefs}
    toplevel .prefs -class Dialog
    wm geometry .prefs =+440+430
    wm title .prefs {Preferences}
    tkwait visibility .prefs
    grab set .prefs
    prefs_ui .prefs
tkwait variable prefsDone
}

# Save the prefs to a file in home directory
proc doSavePrefs {} {
    global serverName printerName portNum prefsDone updateRate
    set f [open "./MUDClient.prefs" w]
    puts $f [concat "ServerName" $serverName]
    puts $f [concat "PortNum" $portNum]
    puts $f [concat "printerName" $printerName]
    puts $f [concat "upDateRate" $updateRate]
    close $f

    # Set the menu bar title!
    catch {wm title .menu [concat {Welcome to MUDClient V1.0 -Connected to} $serverName]}
    set prefsDone 1
}
