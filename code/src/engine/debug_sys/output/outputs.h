#pragma once

#ifndef AE_DEBUG_SYS_OUTPUTS_H
#define AE_DEBUG_SYS_OUTPUTS_H

namespace ae { namespace dbg {

class C_Output;

// ************************************************************************************************************
// Debug output stream modificators
// ************************************************************************************************************

std::ostream& error(std::ostream& os);
std::ostream& info(std::ostream& os);
std::ostream& warning(std::ostream& os);

std::ostream& beep(std::ostream& os);
std::ostream& critical(std::ostream& os);
std::ostream& pid(std::ostream& os);
std::ostream& tid(std::ostream& os);

std::ostream& clr_fmt(std::ostream& os);

// ************************************************************************************************************
// Debug output stream effectors
// ************************************************************************************************************

class src_path
{
	const char* m_sSourceFile;
	unsigned int m_iSourceLine;
public:
	src_path(const char* sourceFile, unsigned int sourceLine);
	friend std::ostream& operator<<(std::ostream& os, const src_path& sp);
};

class color
{
	unsigned int m_Color;
public:
	color(unsigned int c);
	friend std::ostream& operator<<(std::ostream& os, const color& c);
};

class dec_fmt
{
	unsigned int m_Width;
public:
	dec_fmt(unsigned int width);
	friend std::ostream& operator<<(std::ostream& os, const dec_fmt& df);
};

class flt_fmt
{
	int precision;
public:
	flt_fmt(unsigned int precision);
	friend std::ostream& operator<<(std::ostream& os, const flt_fmt& ff);
};
// ************************************************************************************************************
// ************************************************************************************************************

typedef enum E_OutputTags
{
	E_OT_Default,
	E_OT_Error,
	E_OT_Critical,
	E_OT_Warning,
	E_OT_Info,
	E_OT_Beep,
	E_OT_Color,
	E_OT_PID, // Process ID
	E_OT_TID, // Thread ID
	E_OT_Src_File,
	E_OT_Src_Line,
	E_OT_EOT, // End Of Tag tag
	E_OT_EOL, // End Of Line tag
	E_MAX = 0xffff,
};

// ************************************************************************************************************
// ************************************************************************************************************

class C_Output
{
protected:
	C_Output() {};
	virtual ~C_Output() {};

public:
	virtual void OnData(const char * s, std::streamsize n) = 0;
	virtual void OnTag(E_OutputTags, unsigned __int32 tagData) = 0;
};

// ************************************************************************************************************
// C_Outputs redirect debug stream to outputs
// ************************************************************************************************************

class C_Outputs : public std::basic_streambuf<char, std::char_traits<char> >
{
	typedef std::vector<C_Output*> T_Outputs;
	T_Outputs m_Outputs;
	
	virtual std::streamsize xsputn ( const char * s, std::streamsize n ); //!< is called internally from streambuf when ostream parent received data from output.
	virtual int overflow ( int c = EOF ); //!< is called internally from streambuf when ostream parent received data from output.
public:
	C_Outputs();
	~C_Outputs();

	void RegisterOutput(C_Output*);
	void UnregisterOutput(C_Output*);

	void PutTag(E_OutputTags tag, unsigned __int32 tagData = 0);
};

// ************************************************************************************************************
// ************************************************************************************************************

extern C_Outputs doutputs;
extern std::ostream dout;

#define DBG_OUT ae::dbg::dout
#define DBG_ERR ae::dbg::dout << ae::dbg::error
#define DBG_WRN ae::dbg::dout << ae::dbg::warning
#define DBG_INF ae::dbg::dout << ae::dbg::info

#define DBG_SRC_PATH ae::dbg::src_path(__FILE__,__LINE__)

} } // namespace ae { namespace dbg {

#endif // #ifdef AE_DEBUG_SYS_OUTPUTS_H
