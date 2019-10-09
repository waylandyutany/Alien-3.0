#include "StdAfx.h"

#include "outputs.h"

namespace ae { namespace dbg {

// ************************************************************************************************************
// ************************************************************************************************************

std::ostream& clr_fmt(std::ostream& os)
{
	os << std::noshowbase;
	os << std::noshowpos;
	os << std::noshowpoint;
	os << std::nouppercase;
	os << std::noskipws;
	os << std::fixed;
	os << std::internal;

	os << std::setbase(0);
	os << std::setprecision(0);
	os << std::setfill(' ');
	os << std::setw(0);

	return os;
}

std::ostream& error(std::ostream& os)
{
	doutputs.PutTag(E_OT_Error);
	return os;
}

std::ostream& info(std::ostream& os)
{
	doutputs.PutTag(E_OT_Info);
	return os;
}

std::ostream& warning(std::ostream& os)
{
	doutputs.PutTag(E_OT_Warning);
	return os;
}

std::ostream& beep(std::ostream& os)
{
	doutputs.PutTag(E_OT_Beep);
	return os;
}

std::ostream& critical(std::ostream& os)
{
	doutputs.PutTag(E_OT_Critical);
	return os;
}

// ************************************************************************************************************

src_path::src_path( const char* sourceFile, unsigned int sourceLine ) :
	m_sSourceFile(sourceFile),
	m_iSourceLine(sourceLine)
{
}

std::ostream& operator<<(std::ostream& os, const src_path& sp)
{
	doutputs.PutTag(E_OT_Src_File);
	os << sp.m_sSourceFile;

	doutputs.PutTag(E_OT_Src_Line);
	os << "[" << sp.m_iSourceLine << "]";
	
	doutputs.PutTag(E_OT_EOT);
	
	return os;
}

// ************************************************************************************************************

color::color(unsigned int c) :
	m_Color(c)
{
}

std::ostream& operator<<(std::ostream& os, const color& /*c*/)
{
	doutputs.PutTag(E_OT_Color);

	return os;
}

// ************************************************************************************************************

dec_fmt::dec_fmt(unsigned int width) :
	m_Width(width)
{
}

std::ostream& operator<<(std::ostream& os, const dec_fmt& df)
{
	os << std::setw(df.m_Width) << std::setfill('0') << std::internal;
	return os;
}

// ************************************************************************************************************

flt_fmt::flt_fmt(unsigned int precision) : precision(precision) {
}

std::ostream& operator<<(std::ostream& os, const flt_fmt& ff)
{
	os << std::showpos;
	os << std::showpoint;
	os << std::fixed;
	os << std::setprecision(ff.precision);
	return os;
}

// ************************************************************************************************************
// C_Outputs implementation
// ************************************************************************************************************

C_Outputs::C_Outputs()
{
}

C_Outputs::~C_Outputs()
{
}

void C_Outputs::RegisterOutput(C_Output* pOut)
{
	AE_ASSERT(pOut && "Invalid output given!");
	AE_ASSERT(std::find(m_Outputs.begin(),m_Outputs.end(),pOut) == m_Outputs.end() && "Output already registered!");

	m_Outputs.push_back(pOut);
}

void C_Outputs::UnregisterOutput(C_Output* pOut)
{
	AE_ASSERT(pOut && "Invalid output given!");
	T_Outputs::iterator itOut = std::find(m_Outputs.begin(),m_Outputs.end(),pOut);

	AE_ASSERT( itOut != m_Outputs.end() && "Unknown output to unregister!");
	m_Outputs.erase(itOut);
}

/** 
* Distribute received data from ostream parent to registered outputs
*/
std::streamsize C_Outputs::xsputn( const char * s, std::streamsize n )
{
	for(T_Outputs::iterator it = m_Outputs.begin();it != m_Outputs.end(); ++it)
		(*it)->OnData(s,n);

	return n;
}

void C_Outputs::PutTag(E_OutputTags tag, unsigned __int32 tagData)
{
	for(T_Outputs::iterator it = m_Outputs.begin();it != m_Outputs.end(); ++it)
		(*it)->OnTag(tag, tagData);
}

/** 
* Distribute received data from ostream parent to registered outputs
*/
int C_Outputs::overflow ( int c )
{
	char buff[2]; // stacked buffer for int c -> const char* conversion

	buff[0] = (char)c; // int c converted to buf[0]
	buff[1] = 0; // with zero termination

	xsputn(buff,2); // and passed to xsputn virtual method for distribution

	if(buff[0] == '\n')
		PutTag(E_OT_EOL);

	return 1;
}

// ************************************************************************************************************
// ************************************************************************************************************

C_Outputs doutputs;
std::ostream dout(&doutputs);

// ************************************************************************************************************
// ************************************************************************************************************

} } // namespace ae { namespace dbg {
