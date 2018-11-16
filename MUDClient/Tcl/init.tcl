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
set path "C://MUD//MUDClient//Tcl"

# source all script files
puts "Sourcing scripts..."
puts "splash..."
source $path/splash.tcl
doSplash
tkwait visibility .splash
# Make available as proc for debugging
proc doSource {} {
    set path "C://MUD//MUDClient//Tcl"
    
    puts "comms.."
    source $path/comms.tcl
    puts "dialogs..."
    source $path/dialog.tcl
    puts "drawing..."
    source $path/draw.tcl
    puts "prefs..."
    source $path/getPrefs.tcl
    source $path/prefs.ui.tcl
    puts "errors..."
    source $path/mudErrors.tcl
    puts "objects..."
    source $path/objects.tcl
    puts "printing..."
    source $path/print.tcl
    puts "scripts..."
    source $path/script.tcl
    puts "server"
    source $path/server.tcl
    puts "menu bar..."
    source $path/menu.ui.tcl
    puts "toolbar..."
    source $path/toolbar.ui.tcl
    puts "canvas..."
    source $path/canvas.ui.tcl
    puts "object box..."
    source $path/objBox.ui.tcl
    puts "editing..."
    source $path/editBox.ui.tcl
    puts "about..."
    source $path/about.ui.tcl
    puts "other boxes..."
    source $path/scale.ui.tcl
    source $path/rotate.ui.tcl
    puts "Done sourcing..."
    puts ""
}
# Do sourcing
doSource

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

# What happens now is that the client sends a request to the server
# to send it all object data...
puts "Okay... Getting objects from server..."
drawAll all

# All done!
puts "All done! Lets do it!"
catch {destroy .splash}

# This routine should allow us to update the
# local state every n/1000 seconds
# Added 5/2/9doUpdateServer
