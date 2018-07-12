ack-vbox () 
{ 
    author 'Kevin Ernst'
    about 'Search for things inside .vbox files using ack'
    param '$1 - the regex / pattern to search for'
    example ''
    group 'vbox'
    ack --type-set=vbox:ext:vbox,xml \
        --ignore-file=ext:1,2,3,4,5,log,vbox-prev \
        "$1"
}
