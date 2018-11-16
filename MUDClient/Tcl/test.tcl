###########################################
# test.tcl
# Procs to allow easy testing of the script
# running over the network.
# Iain Ollerenshaw 15/2/98
# Version History:
# Date	Who Comments
# 15/2/98	IO	Created
###########################################

source "C://MUD//MUDClient//Tcl//comms.tcl"
global portNum serverName
set portNum  "1992"
set serverName "localhost"
puts "Testing..."
puts "Send.."
stringSend {"0 DRAWALL"}
set result [stringReceive]
puts "Result:"
puts $result