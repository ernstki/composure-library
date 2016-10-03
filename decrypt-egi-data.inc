decrypt-egi-data ()
{
    author 'Kevin Ernst'
    about ''
    param ''
    example ''
    group ''
    for sumsfile in md5_batches/*.sums
    do
        bsub -oo ${sumsfile//.sums}-decrypt.log -N -u kevin.ernst@cchmc.org -M 1000 -W 10:00 <<< "awk '{print $2}' $sumsfile | xargs -i{} -l 1 -n 1 -P 10 -- openssl enc -aes-256-cbc -d -pass file:secret.txt -in {} -out {}.decrypted"
    done
}
