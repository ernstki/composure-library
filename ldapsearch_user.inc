ldapsearch_user ()
{
    author 'Kevin Ernst'
    about 'searches '
    param '$1: the uid to search for'
    #param '$2: attributes to return (default: "cn")'
    example '$ ldapsearch_user ajjfr'
    group 'ldap'
    ldapsearch -LLL -x -y ~/.ldappasswd "(uid=$1)"
}
