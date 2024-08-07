##########################################
# comms.tcl
# Provides Tcl commands for sending/receiving
# strings from server. Directly communicates
# with bound C equivalents.
# Iain Ollerenshaw 17/2/98
# Version History:
# Date	Who Comments
# 17/2/98	IO	Created
###########################################
# This proc sends a string mudString to the client C routine
# which forwards this to the server

proc stringSend {theString} {
    global portNum serverName
    
    puts $theString
    
    if [catch {sendString $theString $portNum $serverName} result] {
        puts stderr $result
        return $result
    } else {
        return 0;
        # Command okay. Result in return value
    }
}

proc stringSendReceive {theString} {
    global portNum serverName
    
    puts $theString
    
    if [catch {sendReceiveString $theString $portNum $serverName} result] {
        return -9
    } else {
        # Command okay. Result in return value
        return $result;
    }
}


# This procedure calls the recieve C routine and
# returns the result.
proc stringReceive {} {
    if [catch {receiveString} result ] {
        return "-9"
    } else {
        # Command okay. Result in return value
        return $result
    }
}

proc closeConnection {} {
    closeClient
}

# This procedure retreives the current users id
proc getID {} {
    if [catch {getUID} result ] {
        puts stderr $result
    } else  {
        # Command okay. Result in return value
        return $result
    }
}
