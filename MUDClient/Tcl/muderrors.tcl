########################################### # mudErrors.tcl
# Routines for error handling.
# Iain Ollerenshaw 17/2/98
# Version History:
# Date	Who Comments
# 17/2/98	IO	Created
###########################################
global OKAY,CREATEERROR,ENTERERROR, OBJECTERROR, DELETEERROR, EXISTERROF global LOCKEDERROR, EDITERROR, COMMANDERROR

# Set up some standard error definitions
proc initErrors {} {
    set OKAY	"1"
    set CREATEERROR
    set ENTERERROR
    set OBJECTERROR
    set DELETEERROR	"-4"
    set EXISTERROR	"-5"
    set LOCKEDERROR	"-6"
    set EDITERROR	"-7"
    set COMMANDERROR "-8"
}

# Inform user of a network script error
proc doScriptError {errorId} {
    global button
    set theError "An error of type "
    set theError [concat $theError $errorId]
    set theError [concat $theError "has occured."]
    # This nasty if block is required as Tcl does not like negative cases in
    # its switch statements. It thinks that they are options... Oh well...
    if {$errorId == -1} {
        set theError [concat $theError "Cannot create object. Is name unique?"]
    } elseif {$errorId == -2} {
        set theError [concat $theError "Cannot enter object. Is name unique?"]
    } elseif {$errorId == -3} {
        set theError [concat $theError "Object error. Why? Who knows!"]
    } elseif {$errorId == -4} {
        set theError [concat $theError "Delete error. Do you own this object?"]
    } elseif {$errorId == "-5"} {
        set theError [concat $theError "Object doesn't exist!"]
    } elseif {$errorId == "-6"} {
        set theError [concat $theError "Object is locked by someone else."]
    } elseif {$errorId == "-7"} {
        set theError [concat $theError "Cannot edit object. Do you own it?"]
    } else {
        set theError [concat $theError "Unknown script command! Oh dear.. You have a problem.."]
    }
    dialog .d {Script Error} $theError {warning} 0 {Oh dear...} Retry
    if {$button == 1} {
        dialog .d {Script Error} {Can't retry... AAaarrrgghhhh!} {warning} 0 Okay!
    }
}
