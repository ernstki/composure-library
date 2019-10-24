kpdiff () 
{ 
    author 'Kevin Ernst'
    about ''
    param ''
    example ''
    group ''
    xml sel -t --var upper=ABCDEFGHIJKLMNOPQRSTUVWXYZ --var lower=abcdefghijklmnopqrstuvwxyz -m '//Entry' -s A:T:- 'translate(String/Key[.="Title"]/../Value, $lower, $upper)' -v 'concat(String/Key[.="Title"]/../Value, "	", String/Key[.="UserName"]/../Value, "	", String/Key[.="Password"]/../Value, "	", String/Key[.="URL"]/../Value)' -n kme.xml
}
