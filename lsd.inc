lsd ()
{
    author 'Kevin Ernst <ernstki -at- mail.uc.edu>'
    about 'List only directories in the c.w.d.'
    param 'none'
    example ''
    group 'filesystem'
    find ./* -maxdepth 0 -type d | sed 's#^\./##'
}
