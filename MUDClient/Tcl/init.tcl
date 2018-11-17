###########################################
# init.tcl
# Contains all code to initalise MUDClient:
# sources script files, gets prefs, inits
# windows and does first server run.
# Iain Ollerenshaw 17/2/98
# Version History:
# Date  Who Comments
# 17/2/98   IO  Created
###########################################
global clientId serverIP selectObj currObject portNum serverName printerName
set clientId 3
set scriptPath [ file dirname [ file normalize [ info script ] ] ]
puts "Determining script path.."
puts $scriptPath

# source all script files
puts "Sourcing scripts..."
puts "splash..."
source $scriptPath/splash.tcl
doSplash
tkwait visibility .splash
# Make available as proc for debugging
proc doSource {scriptPath} {
    
    puts "Path:"
    puts $scriptPath
    
    puts "comms.."
    source $scriptPath/comms.tcl
    puts "dialogs..."
    source $scriptPath/dialog.tcl
    puts "drawing..."
    source $scriptPath/draw.tcl
    puts "prefs..."
    source $scriptPath/getPrefs.tcl
    source $scriptPath/prefs.ui.tcl
    puts "errors..."
    source $scriptPath/mudErrors.tcl
    puts "objects..."
    source $scriptPath/objects.tcl
    puts "printing..."
    source $scriptPath/print.tcl
    puts "scripts..."
    source $scriptPath/script.tcl
    puts "server"
    source $scriptPath/server.tcl
    puts "menu bar..."
    source $scriptPath/menu.ui.tcl
    puts "toolbar..."
    source $scriptPath/toolbar.ui.tcl
    puts "canvas..."
    source $scriptPath/canvas.ui.tcl
    puts "object box..."
    source $scriptPath/objBox.ui.tcl
    puts "editing..."
    source $scriptPath/editBox.ui.tcl
    puts "about..."
    source $scriptPath/about.ui.tcl
    puts "other boxes..."
    source $scriptPath/scale.ui.tcl
    source $scriptPath/rotate.ui.tcl
    puts "Done sourcing..."
    puts ""
}
# Do sourcing
doSource $scriptPath

# Initialise prefs
puts "Getting prefs..."
getPrefs
puts [concat "Welcome user " $clientId]
puts [concat "Server is on " $serverName]
puts ""

if {$clientId == 388} {
    puts "Now don't be an arse or I'll kick your head in"
} elseif {$clientId == 381} {
    puts "Hey me."
} else {
    puts "Hey there!"
}

# Set up toolbar, menu and document windows
puts "Initialising windows..."
puts "Creating toolbar..."

toplevel .tool
wm geometry .tool =+873+165
toolbar_ui .tool
wm title .tool {Tool Bar}
puts "done..."

puts "Creating menubar..."
toplevel .menu
wm geometry .menu =733x23+130+165
menu_ui .menu
wm title .menu [concat {Welcome to MUDClient V1.0 - Connected to} $serverName]
puts "done..."

puts "object box..."
toplevel .object
objBox_ui .object
wm geometry .object =+872+474
wm title .object {Object status}
puts "done..."

puts "Creating doc window..."
testWin
puts "done..."

# Start off on select
puts "Initialising globals..."
set currObject 1
puts "Done..."
puts ""

puts "Tidying up.."
.tool.currObject configure -text Select
set selectObj 1

# All done!
puts "All done! Lets do it!"
catch {destroy .splash}

# What happens now is that the client sends a request to the server
# to send it all object data...
puts "Okay... Getting objects from server..."
drawAll all

# This routine should allow us to update the
# local state every n/1000 seconds
# Added 5/2/9
doUpdateServer
