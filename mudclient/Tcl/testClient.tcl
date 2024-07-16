###########################################
# testClient.tcl
# Procs to allow easy testing of the script
# running over the network.
# Iain Ollerenshaw 15/2/98
# Version History:
# Date	Who Comments
# 15/2/98	IO	Created
###########################################
# Send the string to server
proc stringSend {theString} {
    if [catch {sendString $theString } result] {
        puts stderr $result
    } else {
        puts "Okay..."
        # Command okay. Result in return value
    }
}

# Get string from server
proc stringReceive {} {
    if [catch {receiveString} result ] {
        puts stderr $result
    } else {
        puts "Okay..."
        # Command okay. Result in return value
        return $result
    }
}

# Send dummy string to server
proc tstSend {theString} {
    puts "Sent:"
    puts $theString
}

# Receive dummy result
proc tstRecveive {} {
    # Return a command error
    return "-8"
    # Try it out!
    stringSend {0 DRAW oliCircle}
    set result [stringReceive]
    puts $result
}

