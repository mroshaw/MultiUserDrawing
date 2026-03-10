###########################################
# server.tcl
# Code for continuous server update
# Iain Ollerenshaw 8/3/98
###########################################

# Send update request to server asynchronously
proc doUpdateServer {} {
    global schedID updateRate
    # Send request in background, don't wait for response
    after idle doUpdateServerBackground
    # Schedule next update
    set schedID [after $updateRate doUpdateServer]
}

# Do the actual work via after idle so it doesn't block the UI
proc doUpdateServerBackground {} {
    global objList clientId objData
    # Show busy pointer
    .test configure -cursor {clock}
    # Update objects we don't own
    catch {drawAll notOwn}
    # Restore pointer
    .test configure -cursor {cross}
}

# Terminate the update procedure
proc cancelUpdate {} {
    global schedID
    after cancel $schedID
}