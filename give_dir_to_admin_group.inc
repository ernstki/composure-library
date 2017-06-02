give_dir_to_admin_group ()
{
    author 'Kevin Ernst'
    about 'Gives the name'
    param '$1 (optional) - the directory to grant permissions'
    example 'give_dir_to_admin_group .'
    group 'sysadmin'


    # see which host we're on, and whether a sysadmin group exists
    hostname=$(hostname -s)
    sudo setfacl -m g:oA-SA-bmitfwebp1:rwx .
}
