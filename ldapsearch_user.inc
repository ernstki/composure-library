ldapsearch_user ()
{
    author 'Kevin Ernst'
    about 'searches '
    param '$1: the uid to search for'
    #param '$2: attributes to return (default: "cn")'
    example '$ ldapsearch_user ajjfr'
    group 'ldap'

    local user=$1; shift
    ldapsearch -LLL -x -y ~/.ldappasswd "(uid=$user)" $*
}
