########################################### # print.tcl
# Provides functions to print the canvas
# Iain Ollerenshaw 1/3/98
# Version History:
# Date	Who Comments
# 1/3/98	IO	Created
###########################################
# Perform the printing procedure

proc doPrint {} {
    global printerName
    # Ask if user is sure....
    
    set mess [concat "Print picture to" $printerName]
    set reply [dialog .w Printing... $mess {} 0 Yes No]
    if {$reply == 0} {
        puts "Printing..."
        
        # Generate the ps file
        set thePs [.test.canv postscript]
        
        # Do some fidling to sort Tcl's postscript problem
        # see final report for details
        set theNewPs [concat "%!\n" $thePs]
        
        # Open our files ready for writing
        set f [open /tmp/temp.ps w]
        
        # Save the file
        puts $f $theNewPs
        
        # Close the file
        close $f
        
        # Now do the printing using UNIX 1pr exec chmod a+rx /tmp/temp.ps
        exec 1pr -P$printerName /tmp/temp.ps
    }
}