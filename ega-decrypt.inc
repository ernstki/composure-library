ega-decrypt ()
{
    author 'Kevin Ernst'
    about ''
    param ''
    example ''
    group ''
    openssl enc -aes-256-cbc -d -pass file:secret.txt -in EGAD00000000005/_Affymetrix500K_brlmm96_20070108fs1_Status_Quo_CD.txt.gz.aes -out _Affymetrix500K_brlmm96_20070108fs1_Status_Quo_CD.txt.gz
}
