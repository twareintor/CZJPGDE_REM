///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "cxdisclaimer.h"


int CX_Disclaimer(LPWSTR szwDisc)
{
	
	
	return 0;
}



int General_Disclaimer()
{
	char* szMsgDisc = (char*)GlobalAlloc(GPTR, 0x400);
	// this is a disclaimer from the developers of the 
	memcpy(szMsgDisc + strlen(szMsgDisc), " THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS ``AS IS'' AND ANY EXPRESS  \r\n", 85);
	memcpy(szMsgDisc + strlen(szMsgDisc), " OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES  \r\n", 85);
	memcpy(szMsgDisc + strlen(szMsgDisc), " OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN    \r\n", 85);
	memcpy(szMsgDisc + strlen(szMsgDisc), " NO EVENT SHALL THE FREEBSD PROJECT OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,  \r\n", 85);
	memcpy(szMsgDisc + strlen(szMsgDisc), " INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,\r\n", 85);
	memcpy(szMsgDisc + strlen(szMsgDisc), " BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, \r\n", 85);
	memcpy(szMsgDisc + strlen(szMsgDisc), " DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY  \r\n", 85);
	memcpy(szMsgDisc + strlen(szMsgDisc), " OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING       \r\n", 85);
	memcpy(szMsgDisc + strlen(szMsgDisc), " NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,  \r\n", 85);
	memcpy(szMsgDisc + strlen(szMsgDisc), " EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.                            \r\n", 85);
	
	int nRes = MessageBox(NULL, szMsgDisc, "General disclaimer (from \"FreeBSD\") - PLEASE READ CAREFULLY", MB_OK);
						
	GlobalFree(szMsgDisc);
	return nRes;
}

















///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


