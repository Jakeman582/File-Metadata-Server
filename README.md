## File Metadata Server ##

This is a networking based project where several clients have the ability (once authenticated) to get certain sets of file metadata.

**filed**  
  
This program acts as a daemon that receives requests and returns the appropriate repsonse.  
  
There are 3 types of requests that can be made:  
1.) new key request - The password required to authenticate with the server can be changed.  
2.) file get - The first 100 bytes of the file can be returned.  
3.) file digest - A cryptographic digest of the file using the SHA256 algorithm.  

Example usage: *linux> ./filed &lt;port&gt; &lt;secret key&gt;*  
port - The port to which this server will listen.  
secret key - An alphanumeric password that client programs will need to provide in order to have their requests satisfied.  
  
**newKey**  

This program sets a new password for the filed server created usign the ./filed program.  

Example usage: *./newkey &lt;host&gt; &lt;port&gt; &lt;secret key&gt; &lt;new secret key&gt;*  
host - The machine name where the desired filed server to interact with resides (such as 127.0.0.1).  
port - The port on which the filed server is listening.  
secret key - The secret key needed to authenticate with the filed server.  
new secret key - The new secret key the server should use in order to authenticate requests.  
  
**fileGet**  

This program returns the first 100 bytes of the specified file.  
Any kind of file can be requested (executable, png, txt, etc...).  

Example usage: *./fileGet &lt;host&gt; &lt;port&gt; &lt;secret key&gt; &lt;file name&gt;*  
host - The machine name where the desired filed server to interact with resides (such as 127.0.0.1).  
port - The port on which the filed server is listening.  
secret key - The secret key needed to authenticate with the filed server.  
file name - The requested file.  
  
**fileDigest**  

This program returns an SHA256 cryptographic digest of the requested file.  
Any kind of file can be requested (executable, png, txt, etc...).  

Example usage: *./fileDigest &lt;host&gt; &lt;port&gt; &lt;secret key&gt; &lt;file name&gt;*  
host - The machine name where the desired filed server to interact with resides (such as 127.0.0.1).  
port - The port on which the filed server is listening.  
secret key - The secret key needed to authenticate with the filed server.  
file name - The requested file. 
