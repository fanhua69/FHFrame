

#include "stdafx.h"
#include "string.h"
#include "memory.h"
#include "stdio.h"
#include "FHString.h"
//#include "stdarg.h"  : useless, why did I add it?

static char module_pszDummyText[] = { 0, 0, 0 };

CFHString::CFHString()
{
	m_pszText = module_pszDummyText ;
	m_nTextLength=0;
	m_nBufferLength=0;
};

CFHString::~CFHString()
{
	Release();
};

void CFHString::Release()
{
   if( m_pszText != module_pszDummyText )
   {
      delete [] m_pszText;
	  m_pszText = module_pszDummyText ;
	  m_nBufferLength=0;
	  m_nTextLength=0;
   }
}

CFHString::CFHString( CFHString &other)
{
	m_pszText=module_pszDummyText;
	m_nTextLength=0;
	m_nBufferLength=0;

	CopyConstructor( other );
};

CFHString::CFHString( CFHString *pOther)
{
	m_pszText=module_pszDummyText;
	m_nTextLength=0;
	m_nBufferLength=0;

	CopyConstructor( *pOther );
};


void CFHString::CopyConstructor(CFHString& other )
{
   if( other.m_pszText == module_pszDummyText )
   {
      m_pszText = module_pszDummyText ;
      return;
   }

   AllocateBuffer( other.m_nBufferLength );
   CopyText( other.m_pszText,  other.m_nTextLength );
}


CFHString::CFHString(const char *pszText,unsigned int uintBufferLength)
{
	unsigned int uintTextLength;
	m_pszText=module_pszDummyText;

   // if no text and no length
   if( (pszText == NULL) && (uintBufferLength == 0) )
   {
      // set to our dummy text ( don't need to compare for m_pszText == NULL because of that)
      m_pszText = module_pszDummyText;
      return;
   }
    
   if( pszText == NULL )                                    // if null just make the buffer
   {
	   AllocateBuffer( uintBufferLength );
      return;
   }

   uintTextLength = strlen( pszText );

   // if length i zero and text has been set the make a buffer to the lengt of text
   if( uintBufferLength == 0 )
   {
      uintBufferLength = uintTextLength;
   }

   AllocateBuffer( uintBufferLength );                      // allocate buffer for text

   uintTextLength = Min( uintTextLength, uintBufferLength );// set to text length

   // copy text
   CopyText( pszText, uintTextLength );
};

CFHString & CFHString ::operator=(CFHString & other )
{
   if( &other == this ) return *this;

   CopyConstructor( other );

   return *this;
} 

CFHString & CFHString ::operator=(char *pszText)
{
   if( *this ==pszText) return *this;

   CopyText(pszText);

   return *this;
} 


void CFHString::CopyText(const  char * pszText, unsigned int uintLength )
{
   if( uintLength == 0 )
   {
      uintLength = strlen( pszText );    
   }

   if(m_nBufferLength<uintLength){
	   AllocateBuffer(uintLength);
   }

   memcpy( m_pszText, pszText, uintLength);
   m_pszText[uintLength] = '\0';
   m_nTextLength = uintLength;
}



void CFHString::AllocateBuffer( const int nLength )
{
   if(nLength<1||nLength >CTR_INT_MAX -1||m_nBufferLength==nLength)return;

   // Allocate memory for the data for text and the requested length
   if(m_pszText!= module_pszDummyText){
	   delete []m_pszText;
   }

   m_pszText=new char [nLength + 2];

   m_nBufferLength = nLength;                  // set max length
   m_nTextLength = 0;                             // set current length

   memset(m_pszText,0,nLength+2);
}


/*********************************************************************
* Proc:		CStr::Buffer
* Purpose:	Helper.  Makes sure that our internal buffer has
*			the specified number of bytes available, and that
*			we can overwrite it (i.e. m_Usage is 1).  If this
*			isn't so, prepares a copy of our internal data.
*********************************************************************/
char * CFHString::GetBuffer (int newlength)
{
	CFHString sBak;
	if( newlength > m_nBufferLength ){
		sBak=*this;
		AllocateBuffer( newlength );
		CopyText(sBak);
	}

	return m_pszText;

}

void CFHString::ReleaseBuffer()
{
	if(m_pszText==module_pszDummyText)return;

	CFHString sBak(this);

	Release();

	CopyText(sBak);
}


int CFHString::GetLength()
{
	return m_nTextLength;
}

/* ------------------------------------------------------------------------------------------------
 * NAME:    Add()
 * PURPOSE: Add a string to the current string
 *
 * PARAM:   pszText = the text that will be added to string
 */
void CFHString::Add( char * pszText )
{
   unsigned int uintLength;     // Length of the string we are adding

   uintLength = strlen( pszText );                         // get length 

   // if the buffer has enough room then just append the string
   if( m_nBufferLength > (uintLength + m_nTextLength) )
   {
      Append( pszText, uintLength );
   }
   else
   {
      NewText( m_pszText, pszText, 0 ); // create a new buffer and insert the new text
   }
}

/*********************************************************************
* Proc:		CStr::AddChar
* Purpose:	Appends a single character to the end of the string
*********************************************************************/
void CFHString::AddChar(char ch)
{
	//expand buffer if necessary
	CFHString sBak(this);
	if(m_nBufferLength<m_nTextLength+1){
		AllocateBuffer(m_nBufferLength+10);
	}
	
	// And append the character 
	CopyConstructor(sBak);

	m_pszText[m_nTextLength]=ch;
	m_pszText[m_nTextLength+1]=0;
	m_nTextLength++;
}



void CFHString::Format(const char* fmt, ...)
{
	/*Release();
	// Walk the string
	va_list marker;
	va_start(marker, fmt);
	FormatCore (fmt, marker);
	va_end(marker);
	*/
}


/* ------------------------------------------------------------------------------------------------
 * NAME:    NewText()
 * PURPOSE: Allocates a new buffer and copies text into it. Deletes the old data for the object
 *
 * PARAM:   pszText1      = ponter to the first text that are added to text
 *          pszText2      = points to next text that are added
 *          uintSetLength = the length of the buffer (if 0 the buffer is equal to text)
 */
void CFHString::NewText( char * pszText1, char * pszText2, unsigned int uintSetLength )
{
   unsigned int uintTextLength1;  // length of text 1
   unsigned int uintTextLength2;  // length of text 2
   unsigned int uintTotalLength;  // the length of bothe text 1 and text 2 or the SetLength

   uintTextLength1 = 0;
   uintTextLength2 = 0;

   if( uintSetLength != 0 )
   {
      uintTotalLength = uintSetLength;
   }
   else
   {
      if( pszText1 != NULL ) uintTextLength1 = strlen( pszText1 );
      
      if( pszText2 != NULL ) uintTextLength2 = strlen( pszText2 );

      uintTotalLength = uintTextLength1 + uintTextLength2;
   }

   if( uintTotalLength )
   {
       //  Save this string , because AllocateBuffer will destroy this memory;
	   CFHString s(this);
	   if(pszText1==m_pszText)pszText1=s.GetBuffer();
	   if(pszText2==m_pszText)pszText2=s.GetBuffer();;  

	   AllocateBuffer( uintTotalLength );
       if( pszText1 ) CopyText(pszText1, uintTextLength1 );
       if( pszText2 ) Append  (pszText2, uintTextLength2);
   }

}


void CFHString::Append( char *pszText, unsigned int uintLength )
{
   if( uintLength == 0 )
   {
      uintLength = strlen( pszText );    
   }

   // This  is a private function, and is only for when the buffer length 
   // is bigger what of after appending:
   if(m_nBufferLength < (uintLength + m_nTextLength) )
   {	   
	   return;
   }

   // get position after last character in string
   char * pszPosition = m_pszText + m_nTextLength;
   memcpy( pszPosition, pszText, uintLength * sizeof(char) );
   pszPosition[uintLength] = '\0';

   m_nTextLength += uintLength;
}


/* ------------------------------------------------------------------------------------------------
 * OPERATOR:+=
 * PURPOSE: Adds a text string to the current text
 */
CFHString& CFHString::operator+=( char * pszText )
{
	if(pszText == NULL)return *this;
	Add( pszText );
	return *this;
}

CFHString& CFHString::operator+=( CFHString &text)
{
	Add( text.GetBuffer ());
	return *this;
}


/* ------------------------------------------------------------------------------------------------
 * OPERATOR:+
 * PURPOSE: Adds a text string to an other
 */
CFHString CFHString::operator+( char * pszText )
{
	if(pszText == NULL)return CFHString();

	CFHString sTemp(this);
	sTemp.Add(pszText);
	return sTemp;
}

CFHString CFHString::operator+( CFHString &other)
{
	CFHString sTemp(this);
	sTemp.Add(other.GetBuffer ());
	return sTemp;
}

CFHString operator+(const char* lpsz, CFHString& s)
{
	CFHString temp (lpsz);
	temp += s;
	return temp;
}


/*********************************************************************
* Proc:		operator +char for CFHString
*********************************************************************/
CFHString operator+(char ch, CFHString& s)
{
	CFHString temp (s);
	temp += ch;
	return temp;
}




CFHString operator+( char * pszText,  CFHString &Text  )       
{
	CFHString sTemp(pszText);
	sTemp.Add(Text.GetBuffer());
	return sTemp;
}




/* ------------------------------------------------------------------------------------------------
 * OPERATOR:+=
 * PURPOSE: Convert number and add it to text
 */
CFHString& CFHString::operator+=( long longNumber )
{
   char pszNumber[15];

   sprintf(pszNumber,"%ld",longNumber);
   Add( pszNumber );
	return *this;
}

CFHString CFHString::Mid(unsigned int nStart, unsigned int nChars)
{
	// Nothing to return?
	if (m_nTextLength == 0  || nChars <= 0 ||nStart >= m_nTextLength)
		return CFHString(" ");

	if(nStart<0)nStart=0;

	if ((nStart+nChars) >= m_nTextLength)
		nChars = m_nTextLength - nStart;
	
	// Copy bytes
	CFHString s;
	s.AllocateBuffer (nChars);
	s.Append (&m_pszText[nStart], nChars);

	return s;
}

char CFHString ::GetAt(unsigned int nChar)
{
	if(nChar<0||nChar>=m_nTextLength){
		return '\0';
	}

	return m_pszText[nChar];
}

void CFHString ::SetAt(unsigned int nChar, char cReplace)
{
	if(nChar<0||nChar>=m_nBufferLength){
		return ;
	}

	m_pszText[nChar]=cReplace;
}



CFHString CFHString ::Left(int nLeft)
{
	return Mid(0,nLeft);
}

CFHString CFHString ::Right(int nRight)
{
	unsigned int nStart=m_nTextLength-nRight;
	return Mid(nStart,nRight);	
}

/* ------------------------------------------------------------------------------------------------
 * NAME:    Find()
 * PURPOSE: Find a string in text
 *
 * PARAM:   pszString = the string we are looking for
 *          uintCount = if we don't want to search the whole string then this marks when to stop
 */
int CFHString::Find( char * pszString, unsigned int uintCount )
{
   char  tcharSave;   // Save character if not searching whole text
   char * pszPosition; // Marks where the string was found

   tcharSave = '\0';                                    // no character saved

   if( uintCount && (uintCount < m_nTextLength) )
   {
      tcharSave = m_pszText[uintCount];                     // save character
      m_pszText[uintCount] = '\0';                      // terminate string
   }
                    
   pszPosition = strstr( m_pszText, pszString );           // find string

   if( tcharSave != '\0' )                              // if character saved
   {
      m_pszText[uintCount] = tcharSave;                     // restore character
   }

   // return the index to string if found otherwise -1
   return (pszPosition == NULL ) ? -1 : ( pszPosition - m_pszText );
}	

int CFHString::ReverseFind (char ch, unsigned int startat /*= -1*/)
{
	if (startat == (unsigned int) -1) {
		// Scan entire string
		char* scan = strrchr (m_pszText, ch);
		if (scan)
			return scan - m_pszText;
	}
	else {
		// Make sure the index is OK
		if (startat >= GetLength())
			return -1;

		for (int findex = (int) startat-1; findex >= 0; findex--) {
			if (m_pszText[findex] == ch)
				return findex;
		}
	}
	return -1;
}



bool CFHString::operator == (CFHString &other)
{
   return strcmp(GetBuffer(),other.GetBuffer()) == 0; 
}

bool CFHString::operator != (CFHString &other)
{
   return strcmp(GetBuffer(),other.GetBuffer()) != 0; 
}

bool CFHString::operator < (CFHString &other)
{
   return strcmp(GetBuffer(),other.GetBuffer())< 0; 
}

bool CFHString::operator > (CFHString &other)
{
   return strcmp(GetBuffer(),other.GetBuffer()) > 0; 
}

void CFHString::MakeUpper()
{
	if(m_pszText==module_pszDummyText)return;

	for(int i=0;i<m_nTextLength;i++){
		if(m_pszText[i]>96&&m_pszText[i]<123){
			m_pszText[i]-=32;
		}
	}
}
void CFHString::MakeLower()
{
	if(m_pszText==module_pszDummyText)return;

	for(int i=0;i<m_nTextLength;i++){
		if(m_pszText[i]>64&&m_pszText[i]<91){
			m_pszText[i]+=32;
		}
	}
}

void CFHString::Remove(char c)
{
	if(m_pszText==module_pszDummyText)return;

	int i;
	bool be=true;
	while(be){
		be=false;
		for(i=0;i<m_nTextLength;i++){
			if(m_pszText[i]==c){
				memmove(&m_pszText[i],&m_pszText[i+1],m_nTextLength-i);
				m_nTextLength--;
				be=true;
				break;
			}
		}
	}
}

bool CFHString::Compare(const char* match)
{
	return (strcmp (m_pszText, match));
}

bool CFHString::CompareNoCase (const char* match) const
{
	return (stricmp (m_pszText, match)==0);
}


void CFHString::Trim(const char* charset /*= NULL*/)
{
	TrimLeft(charset );
	TrimRight(charset );
}

/*********************************************************************
* Proc:		CFHString::LTrim
* Purpose:	Remove leading characters from a string.  All characters
*			to be excluded are passed as a parameter; NULL means
*			'truncate spaces'
*********************************************************************/

void CFHString::TrimLeft(const char* charset /*= NULL*/)
{
	unsigned int good = 0;
	if (charset) 
	{
		while (strchr (charset, m_pszText[good]) != NULL)
			good++;
	}
	else 
	{
		int iLen=strlen(m_pszText);
		while (good<iLen&&
			  (m_pszText[good] == ' ' ||
			   m_pszText[good] == 10  ||
			   m_pszText[good] == 13))
		{
			good++;
		}
	}
	if (good > 0)
		RemoveLeft (good);
}


/*********************************************************************
* Proc:		CFHString::Trim
* Purpose:	Remove trailing characters; see LTrim
*********************************************************************/

void CFHString::TrimRight(const char* charset /*= NULL*/)
{
	unsigned int good = strlen(m_pszText);
	
	if (good == 0)
		return;
	
	if (charset) 
	{
		while (good > 0  &&  strchr (charset, m_pszText[good-1]) != NULL)
			--good;
	}
	else
	{
		while (good > 0  
			&&  (m_pszText[good-1] == ' ' 
			|| m_pszText[good-1]==0 
			|| m_pszText[good-1]==10 
			|| m_pszText[good-1]==13))
		{
			--good;
		}
	}

	if (good >= strlen(m_pszText))
		return;

	TruncateAt (good);		// Also works well with good == 0
}

/*********************************************************************
* Proc:		CFHString::RemoveLeft
*********************************************************************/

void CFHString::RemoveLeft(unsigned int count)
{
	if (GetLength() <= count) {
		Release();
		return;
	}
	if (count == 0)
		return;

	memmove (m_pszText, m_pszText+count, m_nTextLength-count+1);
	m_nTextLength=m_nTextLength- count;
}

void CFHString::RemoveMiddle(unsigned int start, unsigned int count)
{
	if (m_nTextLength <= start) {
		Release();
		return;
	}

	char* pstart = m_pszText + start;
	if (m_nTextLength <= (start+count)) {
		pstart[0] = 0;
		m_nTextLength = start;
		return;
	}

	memmove (pstart, pstart+count, m_nTextLength-(start+count)+1);
	m_nTextLength = m_nTextLength- count;
}

void CFHString::RemoveRight(unsigned int count)
{
	if (m_nTextLength <= count)
		Release();
	else
		TruncateAt ((unsigned int)(m_nTextLength - count));
}	

/*********************************************************************
* Proc:		CStr::TruncateAt
* Purpose:	Cuts off the string at the character with the specified
*			index.  The allocated buffer remains the same.
*********************************************************************/
void CFHString::TruncateAt (unsigned int idx)
{
	if (idx >= m_nTextLength)
		return;

	// And do the truncation
	m_pszText[idx] = 0;
	m_nTextLength = idx;
}

