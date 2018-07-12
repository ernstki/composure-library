javardb () 
{ 
    author 'Kevin Ernst'
    about 'Starts Java remote debugging server on port 1044'
    param ''
    example 'java ClassA < inputfile'
    group 'java'
    java -Xdebug -Xrunjdwp:transport=dt_socket,server=y,suspend=y,address=1044 $*
}
