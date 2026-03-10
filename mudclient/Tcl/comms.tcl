###########################################
# comms.tcl
# Provides Tcl commands for sending/receiving
# strings from server. Directly communicates
# with bound C equivalents.
# Iain Ollerenshaw 17/2/98
# Version History:
# Date  Who Comments
# 17/2/98   IO  Created
# 2024      IO  Persistent connection support
###########################################

# Connect to the server once at startup
proc serverConnect {} {
    global portNum serverName
    # puts stdout "Connecting to server $serverName:$portNum..."
    if [catch {connectToServer $portNum $serverName} result] {
        puts stderr "Failed to connect to server: $result"
        tk_messageBox \
            -message "Cannot connect to server $serverName:$portNum" \
            -type ok \
            -icon error
        return -9
    } else {
        # puts stdout "Connected to server $serverName:$portNum"
        return 0
    }
}

# Disconnect from the server on quit
proc serverDisconnect {} {
    catch {disconnectFromServer}
    puts stdout "Disconnected from server."
}

# Send a string to the server (no response expected)
proc stringSend {theString} {
    # puts stdout $theString
    if [catch {sendString $theString} result] {
        puts stderr "stringSend error: $result"
        return -9
    } else {
        return 0
    }
}

# Send a string to the server and receive a response
proc stringSendReceive {theString} {
    # puts stdout $theString
    if [catch {sendReceiveString $theString} result] {
        puts stderr "stringSendReceive error: $result"
        return -9
    } else {
        return $result
    }
}

# Receive a string from the server
proc stringReceive {} {
    if [catch {receiveString} result] {
        puts stderr "stringReceive error: $result"
        return -9
    } else {
        return $result
    }
}

# Get the current user's ID
proc getID {} {
    if [catch {getUID} result] {
        puts stderr "getID error: $result"
        return -9
    } else {
        return $result
    }
}