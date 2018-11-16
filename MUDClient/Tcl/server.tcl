###########################################
# server.tcl
# Code for continuous server update
# Iain Ollerenshaw 8/3/98
# Version History:
# Date	Who Comments
# 8/3/98	IO	Created
###########################################
# Updates the local store and display at intervals
# specified by updateRate
proc doUpdateServer {} {
    global schedID updateRate objList clientId objData
    # Show busy pointer
    .test configure -cursor {clock}
    # What we want to do here is continuously update objects # which we DON'T OWN
    drawAll notOwn
    # Restore pointer
    .test configure -cursor {cross}
    set schedID [after $updateRate doUpdateServer]
}

# Terminate the update procedure or we'll get a
# messy exit
proc cancelUpdate {} {
    global schedID
    after cancel $schedID
}