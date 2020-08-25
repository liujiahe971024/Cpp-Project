1. Suppose we are using a method of encryption that does as much as possible
   to hide any patterns, including character frequencies. Do you believe there
   is any advantage (in terms of resulting file size) to compressing a file
   before encrypting it, as opposed to compressing it after encrypting it?
   In one sentence, say why or why not in the form of a hypothesis.
     

    Yes. It is useless if you compress after encryption because there will be
    no recognizable patterns to exploit after you encrypt the message.


2. Test your hypothesis on the command line using warandpeace.txt,  your 
   compression program, and the command 
   gpg --batch -c -z0 --passphrase <ANY PASSPHRASE YOU WANT> <FILENAME> . 
   The -z0 flag is very important because it prevents GPG from doing its
   own compression. You can read more about GPG at https://www.gnupg.org/. 
   You may use the reference compression program if there is a problem with 
   yours. Answer the following questions:

   a. What file size results from compressing before encrypting with gpg?

     1870800 bytes 

   b. What file size results from compressing after encrypting with gpg?

     3289154 bytes

   c. Do these results agree with your hypothesis?

     Yes, compressing before encrypyting is way smaller than another method.


3. Suppose instead of GPG we encrypted by XORing each byte in the file against
   a single shared one byte key. This is effectively a byte substitution
   cypher. Would this compress better, worse, or the same as a file encrypted 
   with GPG?  In one sentence, why?

    Compress better because after using XORing, the data is still in binary
    model and readable, so we can do compress after encrypt.


4. Would it compress better, worse, or the same as the original
   unencrypted file? In one sentence, why?
    
    Same, because the amount of total binary number is still, so the compress
    file size is same.


5. Would the Shannon entropy be higher, lower, or the same as the original
   unencrypted file? In one sentence, why?

    Same, because the amount of total bytes is still, so the Shannon entropy
    keep same. 

