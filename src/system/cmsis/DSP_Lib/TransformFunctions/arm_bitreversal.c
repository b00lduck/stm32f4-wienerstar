/* ----------------------------------------------------------------------    
* Copyright (C) 2010-2013 ARM Limited. All rights reserved.    
*    
* $Date:        16. October 2013  
* $Revision: 	V1.4.2  
*    
* Project: 	    CMSIS DSP Library    
* Title:	    arm_bitreversal.c    
*    
* Description:	This file has common tables like Bitreverse, reciprocal etc which are used across different functions    
*    
* Target Processor: Cortex-M4/Cortex-M3/Cortex-M0
*  
* Redistribution and use in source and binary forms, with or without 
* modification, are permitted provided that the following conditions
* are met:
*   - Redistributions of source code must retain the above copyright
*     notice, this list of conditions and the following disclaimer.
*   - Redistributions in binary form must reproduce the above copyright
*     notice, this list of conditions and the following disclaimer in
*     the documentation and/or other materials provided with the 
*     distribution.
*   - Neither the name of ARM LIMITED nor the names of its contributors
*     may be used to endorse or promote products derived from this
*     software without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
* "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
* LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
* FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE 
* COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
* INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
* BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
* LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
* CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
* LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
* ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
* POSSIBILITY OF SUCH DAMAGE.  
* -------------------------------------------------------------------- */

#include "arm_math.h"
#include "arm_common_tables.h"

/*    
* @brief  In-place bit reversal function.   
* @param[in, out] *pSrc        points to the in-place buffer of floating-point data type.   
* @param[in]      fftSize      length of the FFT.   
* @param[in]      bitRevFactor bit reversal modifier that supports different size FFTs with the same bit reversal table.   
* @param[in]      *pBitRevTab  points to the bit reversal table.   
* @return none.   
*/

void arm_bitreversal_f32(
float32_t * pSrc,
uint16_t fftSize,
uint16_t bitRevFactor,
uint16_t * pBitRevTab)
{
   uint16_t fftLenBy2, fftLenBy2p1;
   uint16_t i, j;
   float32_t in;

   /*  Initializations */
   j = 0u;
   fftLenBy2 = fftSize >> 1u;
   fftLenBy2p1 = (fftSize >> 1u) + 1u;

   /* Bit Reversal Implementation */
   for (i = 0u; i <= (fftLenBy2 - 2u); i += 2u)
   {
      if(i < j)
      {
         /*  pSrc[i] <-> pSrc[j]; */
         in = pSrc[2u * i];
         pSrc[2u * i] = pSrc[2u * j];
         pSrc[2u * j] = in;

         /*  pSrc[i+1u] <-> pSrc[j+1u] */
         in = pSrc[(2u * i) + 1u];
         pSrc[(2u * i) + 1u] = pSrc[(2u * j) + 1u];
         pSrc[(2u * j) + 1u] = in;

         /*  pSrc[i+fftLenBy2p1] <-> pSrc[j+fftLenBy2p1] */
         in = pSrc[2u * (i + fftLenBy2p1)];
         pSrc[2u * (i + fftLenBy2p1)] = pSrc[2u * (j + fftLenBy2p1)];
         pSrc[2u * (j + fftLenBy2p1)] = in;

         /*  pSrc[i+fftLenBy2p1+1u] <-> pSrc[j+fftLenBy2p1+1u] */
         in = pSrc[(2u * (i + fftLenBy2p1)) + 1u];
         pSrc[(2u * (i + fftLenBy2p1)) + 1u] =
         pSrc[(2u * (j + fftLenBy2p1)) + 1u];
         pSrc[(2u * (j + fftLenBy2p1)) + 1u] = in;

      }

      /*  pSrc[i+1u] <-> pSrc[j+1u] */
      in = pSrc[2u * (i + 1u)];
      pSrc[2u * (i + 1u)] = pSrc[2u * (j + fftLenBy2)];
      pSrc[2u * (j + fftLenBy2)] = in;

      /*  pSrc[i+2u] <-> pSrc[j+2u] */
      in = pSrc[(2u * (i + 1u)) + 1u];
      pSrc[(2u * (i + 1u)) + 1u] = pSrc[(2u * (j + fftLenBy2)) + 1u];
      pSrc[(2u * (j + fftLenBy2)) + 1u] = in;

      /*  Reading the index for the bit reversal */
      j = *pBitRevTab;

      /*  Updating the bit reversal index depending on the fft length  */
      pBitRevTab += bitRevFactor;
   }
}



/*    
* @brief  In-place bit reversal function.   
* @param[in, out] *pSrc        points to the in-place buffer of Q31 data type.   
* @param[in]      fftLen       length of the FFT.   
* @param[in]      bitRevFactor bit reversal modifier that supports different size FFTs with the same bit reversal table   
* @param[in]      *pBitRevTab  points to bit reversal table.   
* @return none.   
*/

void arm_bitreversal_q31(
q31_t * pSrc,
uint32_t fftLen,
uint16_t bitRevFactor,
uint16_t * pBitRevTable)
{
   uint32_t fftLenBy2, fftLenBy2p1, i, j;
   q31_t in;

   /*  Initializations      */
   j = 0u;
   fftLenBy2 = fftLen / 2u;
   fftLenBy2p1 = (fftLen / 2u) + 1u;

   /* Bit Reversal Implementation */
   for (i = 0u; i <= (fftLenBy2 - 2u); i += 2u)
   {
      if(i < j)
      {
         /*  pSrc[i] <-> pSrc[j]; */
         in = pSrc[2u * i];
         pSrc[2u * i] = pSrc[2u * j];
         pSrc[2u * j] = in;

         /*  pSrc[i+1u] <-> pSrc[j+1u] */
         in = pSrc[(2u * i) + 1u];
         pSrc[(2u * i) + 1u] = pSrc[(2u * j) + 1u];
         pSrc[(2u * j) + 1u] = in;

         /*  pSrc[i+fftLenBy2p1] <-> pSrc[j+fftLenBy2p1] */
         in = pSrc[2u * (i + fftLenBy2p1)];
         pSrc[2u * (i + fftLenBy2p1)] = pSrc[2u * (j + fftLenBy2p1)];
         pSrc[2u * (j + fftLenBy2p1)] = in;

         /*  pSrc[i+fftLenBy2p1+1u] <-> pSrc[j+fftLenBy2p1+1u] */
         in = pSrc[(2u * (i + fftLenBy2p1)) + 1u];
         pSrc[(2u * (i + fftLenBy2p1)) + 1u] =
         pSrc[(2u * (j + fftLenBy2p1)) + 1u];
         pSrc[(2u * (j + fftLenBy2p1)) + 1u] = in;

      }

      /*  pSrc[i+1u] <-> pSrc[j+1u] */
      in = pSrc[2u * (i + 1u)];
      pSrc[2u * (i + 1u)] = pSrc[2u * (j + fftLenBy2)];
      pSrc[2u * (j + fftLenBy2)] = in;

      /*  pSrc[i+2u] <-> pSrc[j+2u] */
      in = pSrc[(2u * (i + 1u)) + 1u];
      pSrc[(2u * (i + 1u)) + 1u] = pSrc[(2u * (j + fftLenBy2)) + 1u];
      pSrc[(2u * (j + fftLenBy2)) + 1u] = in;

      /*  Reading the index for the bit reversal */
      j = *pBitRevTable;

      /*  Updating the bit reversal index depending on the fft length */
      pBitRevTable += bitRevFactor;
   }
}



/*    
   * @brief  In-place bit reversal function.   
   * @param[in, out] *pSrc        points to the in-place buffer of Q15 data type.   
   * @param[in]      fftLen       length of the FFT.   
   * @param[in]      bitRevFactor bit reversal modifier that supports different size FFTs with the same bit reversal table   
   * @param[in]      *pBitRevTab  points to bit reversal table.   
   * @return none.   
*/

void arm_bitreversal_q15(
q15_t * pSrc16,
uint32_t fftLen,
uint16_t bitRevFactor,
uint16_t * pBitRevTab)
{
   q31_t *pSrc = (q31_t *) pSrc16;
   q31_t in;
   uint32_t fftLenBy2, fftLenBy2p1;
   uint32_t i, j;

   /*  Initializations */
   j = 0u;
   fftLenBy2 = fftLen / 2u;
   fftLenBy2p1 = (fftLen / 2u) + 1u;

   /* Bit Reversal Implementation */
   for (i = 0u; i <= (fftLenBy2 - 2u); i += 2u)
   {
      if(i < j)
      {
         /*  pSrc[i] <-> pSrc[j]; */
         /*  pSrc[i+1u] <-> pSrc[j+1u] */
         in = pSrc[i];
         pSrc[i] = pSrc[j];
         pSrc[j] = in;

         /*  pSrc[i + fftLenBy2p1] <-> pSrc[j + fftLenBy2p1];  */
         /*  pSrc[i + fftLenBy2p1+1u] <-> pSrc[j + fftLenBy2p1+1u] */
         in = pSrc[i + fftLenBy2p1];
         pSrc[i + fftLenBy2p1] = pSrc[j + fftLenBy2p1];
         pSrc[j + fftLenBy2p1] = in;
      }

      /*  pSrc[i+1u] <-> pSrc[j+fftLenBy2];         */
      /*  pSrc[i+2] <-> pSrc[j+fftLenBy2+1u]  */
      in = pSrc[i + 1u];
      pSrc[i + 1u] = pSrc[j + fftLenBy2];
      pSrc[j + fftLenBy2] = in;

      /*  Reading the index for the bit reversal */
      j = *pBitRevTab;

      /*  Updating the bit reversal index depending on the fft length  */
      pBitRevTab += bitRevFactor;
   }
}



void arm_bitreversal_32(uint32_t *pSrc, const uint16_t bitRevLen, const uint16_t * pBitRevTable) {

    if (bitRevLen < 1) return;

    uint16_t* tablePointer = (uint16_t*) pBitRevTable;

    uint16_t length = bitRevLen >> 2;

    while(length--) {

    	// get the offsets from the table
    	uint32_t deref1 = *(tablePointer + 3);
    	uint32_t deref2 = *(tablePointer + 2);
    	uint32_t deref3 = *(tablePointer + 1);
    	uint32_t deref4 = *(tablePointer);

    	// create the source pointers by adding source address to offsets from table
		deref1 += (uint32_t) pSrc; // ADD      r8,r0,r8
		deref2 += (uint32_t) pSrc; // ADD      r9,r0,r9
		deref3 += (uint32_t) pSrc; // ADD      r2,r0,r2
		deref4 += (uint32_t) pSrc; // ADD      r12,r0,r12

		uint32_t a1,a2,a3,a4;

		a1 = *(uint32_t*)deref2; // LDR      r7,[r9,#0]
		a2 = *(uint32_t*)deref1; // LDR      r6,[r8,#0]
		a3 = *(uint32_t*)deref3; // LDR      r5,[r2,#0]
		a4 = *(uint32_t*)deref4; // LDR      r4,[r12,#0]

		*(uint32_t*)deref2 = a2; // STR      r6,[r9,#0]
		*(uint32_t*)deref1 = a1; // STR      r7,[r8,#0]
		*(uint32_t*)deref3 = a4; // STR      r5,[r12,#0]
		*(uint32_t*)deref4 = a3; // STR      r4,[r2,#0]

		deref1 += 4;
		deref2 += 4;
		deref3 += 4;
		deref4 += 4;

		a1 = *(uint32_t*)deref2; // LDR      r7,[r9,#4]
		a2 = *(uint32_t*)deref1; // LDR      r6,[r8,#4]
		a3 = *(uint32_t*)deref3; // LDR      r5,[r2,#4]
		a4 = *(uint32_t*)deref4; // LDR      r4,[r12,#4]

		*(uint32_t*)deref2 = a2; // STR      r6,[r9,#4]
		*(uint32_t*)deref1 = a1; // STR      r7,[r8,#4]
		*(uint32_t*)deref3 = a4; // STR      r5,[r12,#4]
		*(uint32_t*)deref4 = a3; // STR      r4,[r2,#4]

	    tablePointer += 4;
    }

}
