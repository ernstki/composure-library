is_ppc_arch ()
{
    author 'Kevin Ernst <ernstki -at- mail.uc.edu>'
    about "Determine if the current system's architecture is PPC"
    param ''
    example ''
    group 'build'
    uname -a | awk '{print $9,$10,$11}' | grep -q ppc
}
