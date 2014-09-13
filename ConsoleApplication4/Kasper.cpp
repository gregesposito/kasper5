//#define _CRT_SECURE_NO_WARNINGS 
// Need OpenSSL Libs linked, headers linked, dlls linked
#include <stdio.h>
#include <string.h>
#include <openssl/md5.h>
#include <iostream>
#include <iomanip>      // std::setprecision
#include <ctime>

using namespace std;
int main(int argc, char* argv[]) 
{ 
	/* Start - Setup Timer */
		
	std::clock_t start;
    double duration;
    start = std::clock();

	/* End - Setup Timer */

	/* Start - Hash Length Check */ //Remove for a small ~10k jump

	size_t j;
	for(int i = 1; i < argc; i++) {
	  j = strlen(argv[i]);
	if (j != 10){
			std::cout<<argv[i]<<" is "<<j<<" characters long - not 10! Quitting\n";
			return 0;}
	}

	/* End - Hash Length Check */

	/* Start - Line Entry and Count */
     char string[40]; //Find max length of passwords - TODO
	 size_t i;
	 __int64 linecount = 0; // caps at 9,223,372,036,854,775,807
	 int billioncount = 0;
//  printf("Enter a string: ");
  while(fgets(string, 40, stdin)){

  /* remove newline, if present */
  i = strlen(string)-1;
  if( string[i] == '\n') 
      string[i] = '\0';

  linecount++;
  if (linecount == 10000000){
	  duration = (std::clock() - start) / (double)CLOCKS_PER_SEC;
	  double linespersec = linecount / duration;
	  std::cout << fixed << setprecision(0) << "10 million tries (" << linespersec << " l/s)\n";
  }
  if (linecount == 100000000){
	  duration = (std::clock() - start) / (double)CLOCKS_PER_SEC;
	  double linespersec = linecount / duration;
	  std::cout << fixed << setprecision(0) << "100 million tries (" << linespersec << " l/s)\n";
  }
  if (linecount % 1000000000 == 0){
	  billioncount++;
	  duration = (std::clock() - start) / (double)CLOCKS_PER_SEC;
	  double linespersec = linecount / duration;
	  std::cout << fixed << setprecision(0) << billioncount << " billion tries (" << linespersec << " l/s)\n";
  }
	/* End- Line Entry */

  /* Start - MD5 Round 1 */

   unsigned char digest[16];
    char string2[5];	 

	MD5_CTX ctx;
	MD5_Init(&ctx);
	MD5_Update(&ctx, string, strlen(string));
	MD5_Final(digest, &ctx);

	/* End - MD5 Round 1 */
 
	/* Start - MD5 Round 2 */

// Set the string to the second MD5 hash of the first 5 characters (10 bit)
	memcpy(string2, digest, 5);

	MD5_Init(&ctx);
	MD5_Update(&ctx, string2, 5);
	MD5_Final(digest, &ctx);

    char mdString3[10]; // Swaped from 33 to 10. MD5 hash value is expressed as a hex number, 32 digits long. In this tutorial, the allocation of 33 will give 1 additional space for null terminated string in C++.

	//This method more than doubles the speed versus sprintf, which was 25% faster than a for loop
	char *hexStr = "0123456789abcdef";
	//for (int i = 0; i < 5; i++)
	//{
	//	mdString3[i * 2] = hexStr[(digest[i] >> 4) & 0xf];
	//	mdString3[i * 2 + 1] = hexStr[(digest[i]) & 0xf];
	//}

	mdString3[0] = hexStr[(digest[0] >> 4) & 0xf];
	mdString3[1] = hexStr[(digest[0]) & 0xf];
	mdString3[2] = hexStr[(digest[1] >> 4) & 0xf];
	mdString3[3] = hexStr[(digest[1]) & 0xf];
	mdString3[4] = hexStr[(digest[2] >> 4) & 0xf];
	mdString3[5] = hexStr[(digest[2]) & 0xf];
	mdString3[6] = hexStr[(digest[3] >> 4) & 0xf];
	mdString3[7] = hexStr[(digest[3]) & 0xf];
	mdString3[8] = hexStr[(digest[4] >> 4) & 0xf];
	mdString3[9] = hexStr[(digest[4]) & 0xf];
	
	//mdString3[10] = 0;
	//sprintf(&mdString3[0],"%02x%02x%02x%02x%02x", (unsigned char)digest[0],	(unsigned char)digest[1], (unsigned char)digest[2], (unsigned char)digest[3], (unsigned char)digest[4]);

	//cout << mdString3[0] <<mdString3[1] << mdString3[2] << mdString3[3] << mdString3[4] << mdString3[5] << mdString3[6] << mdString3[7] << mdString3[8] << mdString3[9];
	//printf("1 at: %s", mdString3);
	//printf("2 at: %s", argv[1]);
	
	/* End - MD5 Round 2 */

	/* Start - Hash Check */
for(int i = 1; i < argc; i++) {

	if (memcmp(mdString3, argv[i], 10) == 0){
			printf("Success at: %s", string);
    duration = ( std::clock() - start ) / (double) CLOCKS_PER_SEC;
	double linespersec = linecount/duration;
	std::cout << fixed << setprecision(0) << " for " << argv[i] << " in " << duration << " seconds at line " << linecount << " (" << linespersec << " l/s)\n";
    }
}
  }
    /* End - Hash Check*/

  /* Start - Timer Closeout */
	duration = ( std::clock() - start ) / (double) CLOCKS_PER_SEC;
		double linespersec = linecount/duration;
		std::cout << fixed << setprecision(0) << "Exhausted search of " << linecount << " lines in " << duration << " seconds (" << linespersec << " l/s)\n";
/* End - Timer Closeout */
    return 0;
}