#pragma once

#include <stdint.h>

namespace {
template <  bool saveRa,									//if true, puts a or at, ra, r0 before the 3 op jalr
			bool link,										//if true, use jal instruction (link); else uses jr instruction (no link)
			const uint8_t delaySlot[4],						//op that was in delayslot of hooked jal
			int additionalNops,								//adds more nops after the redirect, e.g to mask a delay slot
			const int injectTargetBytes[4],					//4 bytes of redirect code that contains absolute address to jump to
			unsigned int injectCodeSize,					//size of custom code to be injected
			const uint8_t injectCode[injectCodeSize] >		//array containing custom code to be injected
class InjectedCode {
private:
	InjectedCode() = delete;
protected:
	//binary of lui at, 0xFFFF; ori at, at, 0xFFFF; jalr at;
	//annotated bytes are target address
	static constexpr uint8_t mipsRegJal[12] = {
		0x3C, 0x01, /*<*/0xFF, 0xFF/*>*/, 0x34, 0x21, /*<*/0xFF, 0xFF,/*>*/0x00, 0x20, 0xF8, 0x09
	};
	//binary of lui at, 0xFFFF; ori at, at, 0xFFFF; jr at;
	//annotated bytes are target address
	static constexpr uint8_t mipsRegJ[12] = {
		0x3C, 0x01, /*<*/0xFF, 0xFF/*>*/, 0x34, 0x21, /*<*/0xFF, 0xFF,/*>*/0x00, 0x20, 0x00, 0x08
	};
	static constexpr int mipsRegJal_TargetBytes[4] = { 2,3, 6,7 };
	//binary of or t9, ra, r0
	static constexpr uint8_t mipsRaSave[4] = {
		0x03, 0xE0, 0xC8, 0x25
	};
public:


	static uint8_t* CreateRedirect(unsigned int* outSize = nullptr) {		//builds out of mipsRegJal and the specific delay slot
		uint8_t* buf = new uint8_t[16 + additionalNops * 4 + saveRa * 4];
		if (saveRa) {
			memcpy_s(buf, 4, mipsRaSave, 4);
		}
		if (link) {
			memcpy_s(buf + saveRa * 4, 12, mipsRegJal, 12);
		}
		else {
			memcpy_s(buf + saveRa * 4, 12, mipsRegJ, 12);
		}
		memcpy_s(buf + saveRa * 4 + 12, 4, delaySlot, 4);
		if (additionalNops > 0) {
			memset(buf + saveRa * 4 + 16, 0, additionalNops * 4);
		}
		if (outSize != nullptr) *outSize = 16 + additionalNops * 4 + saveRa * 4;
		return buf;
	}

	static void SetRedirectTarget(uint8_t* buf, uint32_t replacement) {
		buf[mipsRegJal_TargetBytes[0] + saveRa * 4] = replacement >> 24 & 0xFF;
		buf[mipsRegJal_TargetBytes[1] + saveRa * 4] = replacement >> 16 & 0xFF;
		buf[mipsRegJal_TargetBytes[2] + saveRa * 4] = replacement >> 8 & 0xFF;
		buf[mipsRegJal_TargetBytes[3] + saveRa * 4] = replacement >> 0 & 0xFF;
	}


	static uint8_t* CreateInjection(unsigned int* outSize = nullptr) {
		uint8_t* buf = new uint8_t[injectCodeSize];
		memcpy_s(buf, injectCodeSize, injectCode, injectCodeSize);
		if (outSize != nullptr) *outSize = injectCodeSize;
		return buf;
	}

	static void SetInjectionTableAddress(uint8_t* buf, uint32_t tableAddr) {
		if (injectTargetBytes[0] >= 0)
			buf[injectTargetBytes[0]] = tableAddr >> 24 & 0xFF;
		if (injectTargetBytes[1] >= 0)
			buf[injectTargetBytes[1]] = tableAddr >> 16 & 0xFF;
		if (injectTargetBytes[2] >= 0)
			buf[injectTargetBytes[2]] = tableAddr >> 8 & 0xFF;
		if (injectTargetBytes[3] >= 0)
			buf[injectTargetBytes[3]] = tableAddr >> 0 & 0xFF;
	}

	//note jal limitations
	static void SetJalHelperAddress(uint8_t* buf, uint32_t targetAddr) {
		if (targetAddr & 0b11) {
			//lower two bits must be unset
			throw std::string("misaligned jal target address");
		}
		targetAddr >>= 2;
		if (injectTargetBytes[0] >= 0)
			buf[injectTargetBytes[0]] = (buf[injectTargetBytes[0]] & ~0x2) | (targetAddr >> 24 & 0xFF & 0x2);
		if (injectTargetBytes[1] >= 0)
			buf[injectTargetBytes[1]] = targetAddr >> 16 & 0xFF;
		if (injectTargetBytes[2] >= 0)
			buf[injectTargetBytes[2]] = targetAddr >> 8 & 0xFF;
		if (injectTargetBytes[3] >= 0)
			buf[injectTargetBytes[3]] = targetAddr >> 0 & 0xFF;
	}
};

constexpr int Injection_NoTableAddrBytes[4] = { -1,-1,-1,-1 };
};



///////////////

constexpr uint32_t rentalRedirectAdr = 0xFA740;
constexpr uint8_t rentalDelaySlot[4] = {
	0x00, 0x40, 0x28, 0x25 //or a1, v0, r0
};
//annotated bytes are custom rental table address
constexpr uint8_t rentalLoadInjection[216] = {
	0x3C, 0x09, 0x80, 0x14, 0x35, 0x29, 0x51, 0xD0, 0x95, 0x29, 0x00, 0x00,
	0x3C, 0x08, /*<*/0xDF, 0xFF,/*>*/ 0x35, 0x08, /*<*/0xFF, 0xFF,/*>*/ 0x8D, 0x0A, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x01, 0x2A, 0x08, 0x23, 0x04, 0x20, 0xFF, 0xFC,
	0x25, 0x08, 0x00, 0x0C, 0x25, 0x08, 0xFF, 0xF4, 0x8D, 0x09, 0x00, 0x04,
	0x8D, 0x0A, 0x00, 0x08, 0x25, 0x29, 0x00, 0x01, 0x11, 0x20, 0x00, 0x19,
	0x25, 0x29, 0xFF, 0xFF, 0x00, 0x05, 0x59, 0x00, 0x00, 0x8B, 0x60, 0x21,
	0x27, 0xBD, 0xFF, 0xF0, 0xAF, 0xB0, 0x00, 0x00, 0xAF, 0xB1, 0x00, 0x04,
	0xAF, 0xAC, 0x00, 0x08, 0xAF, 0xBF, 0x00, 0x0C, 0x8D, 0x90, 0x00, 0x10,
	0x8D, 0x91, 0x00, 0x14, 0xAD, 0x89, 0x00, 0x10, 0xAD, 0x8A, 0x00, 0x14,
	0x3C, 0x08, 0x80, 0x07, 0x25, 0x08, 0x1B, 0x20, 0x01, 0x00, 0xF8, 0x09,
	0x00, 0x00, 0x00, 0x00, 0x8F, 0xAC, 0x00, 0x08, 0xAD, 0x90, 0x00, 0x10,
	0xAD, 0x91, 0x00, 0x14, 0x8F, 0xB0, 0x00, 0x00, 0x8F, 0xB1, 0x00, 0x04,
	0x8F, 0xBF, 0x00, 0x0C, 0x27, 0xBD, 0x00, 0x10, 0x10, 0x00, 0x00, 0x09,
	0x00, 0x00, 0x00, 0x00, 0x27, 0xBD, 0xFF, 0xFC, 0xAF, 0xBF, 0x00, 0x00,
	0x3C, 0x08, 0x80, 0x07, 0x25, 0x08, 0x1B, 0x20, 0x01, 0x00, 0xF8, 0x09,
	0x00, 0x00, 0x00, 0x00, 0x8F, 0xBF, 0x00, 0x00, 0x27, 0xBD, 0x00, 0x04,
	0x26, 0x01, 0x7F, 0xFF, 0xAC, 0x22, 0x21, 0x79, 0x3C, 0x08, 0x80, 0x00,
	0x25, 0x08, 0x2B, 0x24, 0x01, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00
};

constexpr int rentalLoadInjection_TableAddrBytes[4] = {14, 15, 18, 19};

///////////////

constexpr uint32_t itemRedirectAdr = 0xD9630; //until excluding 68C
constexpr uint8_t itemDelaySlot[4] = {
	0x00,0x00,0x00,0x00 //nops, no delay slot needed
};
constexpr uint8_t itemLoadInjection[164] = {
	0x27, 0xBD, 0xFF, 0xF0, 0xAF, 0xBF, 0x00, 0x0C, 0xAF, 0xB2, 0x00, 0x08,
	0xAF, 0xB3, 0x00, 0x04, 0xAF, 0xB4, 0x00, 0x00, 0x3C, 0x09, 0x80, 0x14,
	0x35, 0x29, 0x51, 0xD0, 0x95, 0x29, 0x00, 0x00, 0x3C, 0x08, /*<*/0xDF, 0xFF,/*>*/
	0x35, 0x08, /*<*/0xFF, 0xFF,/*>*/ 0x8D, 0x0A, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x01, 0x2A, 0x08, 0x23, 0x04, 0x20, 0xFF, 0xFC, 0x25, 0x08, 0x00, 0x08,
	0x25, 0x08, 0xFF, 0xF8, 0x8D, 0x12, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00,
	0x8E, 0x53, 0x00, 0x00, 0x26, 0x52, 0x00, 0x04, 0x34, 0x14, 0x00, 0x18,
	0x02, 0x93, 0x48, 0x06, 0x31, 0x29, 0x00, 0xFF, 0x11, 0x20, 0x00, 0x0A,
	0x8F, 0xA1, 0x00, 0x0C, 0x02, 0x00, 0x20, 0x25, 0x01, 0x20, 0x28, 0x25,
	0x24, 0x21, 0xFF, 0x20, 0x00, 0x20, 0xF8, 0x09, 0x02, 0x20, 0x30, 0x25,
	0x12, 0x80, 0xFF, 0xF3, 0x26, 0x94, 0xFF, 0xF8, 0x10, 0x00, 0xFF, 0xF4,
	0x00, 0x00, 0x00, 0x00, 0x8F, 0xB4, 0x00, 0x00, 0x8F, 0xB3, 0x00, 0x04,
	0x8F, 0xB2, 0x00, 0x08, 0x8F, 0xBF, 0x00, 0x0C, 0x27, 0xBD, 0x00, 0x10,
	0x03, 0xE0, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00
};


constexpr int itemLoadInjection_TableAddrBytes[4] = { 34,35, 38,39 };


///////////////

constexpr uint32_t faceRedirectAdr = 0x142758;
constexpr uint8_t faceDelaySlot[4] = {
	0x26, 0x31, 0x00, 0x01 //addiu s1, s1, 0x0001
};
//annotated bytes are custom rental table address
constexpr uint8_t faceLoadInjection[188] = {
	0x00, 0x00, 0x20, 0x25, 0x3C, 0x08, 0xDF, 0xFF, 0x35, 0x08, 0xFF, 0xFF,
	0x24, 0xA9, 0xFF, 0xBD, 0x05, 0x20, 0x00, 0x1F, 0x00, 0x09, 0x48, 0xC0,
	0x01, 0x28, 0x48, 0x21, 0x00, 0x00, 0x28, 0x25, 0x3C, 0x0C, 0x80, 0x09,
	0x8D, 0x8C, 0x71, 0x20, 0x00, 0x00, 0x00, 0x00, 0x8D, 0x8C, 0x00, 0x28,
	0x27, 0xBD, 0xFF, 0xF0, 0x8D, 0x8A, 0x00, 0x10, 0x8D, 0x8B, 0x00, 0x14,
	0xAF, 0xAC, 0x00, 0x0C, 0xAF, 0xAA, 0x00, 0x08, 0xAF, 0xAB, 0x00, 0x04,
	0xAF, 0xBF, 0x00, 0x00, 0x8D, 0x2A, 0x00, 0x00, 0x8D, 0x2B, 0x00, 0x04,
	0xAD, 0x8A, 0x00, 0x10, 0xAD, 0x8B, 0x00, 0x14, 0x3C, 0x08, 0x80, 0x05,
	0x25, 0x08, 0xE0, 0xF0, 0x01, 0x00, 0xF8, 0x09, 0x00, 0x00, 0x00, 0x00,
	0x8F, 0xAC, 0x00, 0x0C, 0x8F, 0xA9, 0x00, 0x08, 0x8F, 0xAA, 0x00, 0x04,
	0x8F, 0xBF, 0x00, 0x00, 0xAD, 0x89, 0x00, 0x10, 0xAD, 0x8A, 0x00, 0x14,
	0x27, 0xBD, 0x00, 0x10, 0x10, 0x00, 0x00, 0x09, 0x00, 0x00, 0x00, 0x00,
	0x27, 0xBD, 0xFF, 0xFC, 0xAF, 0xBF, 0x00, 0x00, 0x3C, 0x01, 0x80, 0x05,
	0x24, 0x21, 0xE0, 0xF0, 0x00, 0x20, 0xF8, 0x09, 0x00, 0x00, 0x00, 0x00,
	0x8F, 0xBF, 0x00, 0x00, 0x27, 0xBD, 0x00, 0x04, 0xAE, 0x82, 0x00, 0x1C,
	0x03, 0xE0, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00
};


constexpr int faceLoadInjection_TableAddrBytes[4] = { 6, 7, 10, 11 };

///////////////

constexpr uint32_t face2RedirectAdr = 0x4ECF0;
constexpr uint8_t face2DelaySlot[4] = {
	0x00, 0x00, 0x00, 0x00 //nop
};
//annotated bytes are custom rental table address
constexpr uint8_t face2LoadInjection[128] = {
	0x03, 0xE0, 0x68, 0x25, 0x03, 0x20, 0xF8, 0x25, 0x3C, 0x0C, 0x80, 0x09,
	0x8D, 0x8C, 0x71, 0x20, 0x00, 0x00, 0x00, 0x00, 0x8D, 0x8C, 0x00, 0x28,
	0x00, 0x00, 0x00, 0x00, 0x24, 0xA9, 0xFF, 0xBD, 0x05, 0x20, 0x00, 0x0B,
	0x3C, 0x08, 0xDF, 0xFF, 0x35, 0x08, 0xFF, 0xFF, 0x00, 0x09, 0x48, 0xC0,
	0x01, 0x28, 0x48, 0x21, 0x00, 0x00, 0x28, 0x25, 0x8D, 0x2A, 0x00, 0x00,
	0x8D, 0x2B, 0x00, 0x04, 0xAD, 0x8A, 0x00, 0x10, 0xAD, 0x8B, 0x00, 0x14,
	0x10, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x25,
	0x34, 0x08, 0x04, 0x40, 0xAD, 0x88, 0x00, 0x10, 0x34, 0x09, 0x60, 0x30,
	0xAD, 0x89, 0x00, 0x14, 0x27, 0xBD, 0xFF, 0xC0, 0x3C, 0x02, 0x80, 0x09,
	0x8C, 0x42, 0x71, 0x20, 0xAF, 0xB0, 0x00, 0x18, 0x00, 0x80, 0x80, 0x25,
	0x01, 0xA0, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00
};

constexpr int face2LoadInjection_TableAddrBytes[4] = { 38, 39, 42, 43 };

///////////////

//has to be inserted manually, no code injection is associated
constexpr uint8_t stringHelperFindTable[68] = {
	0x3C, 0x0C, 0xDF, 0xFF, 0x35, 0x8C, 0xFF, 0xFF, 0x01, 0x80, 0x10, 0x25,
	0x00, 0x00, 0x48, 0x24, 0x35, 0x29, 0x00, 0xFF, 0x8C, 0x4A, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x0A, 0x5C, 0x02, 0x11, 0x64, 0x00, 0x05,
	0x00, 0x00, 0x00, 0x00, 0x15, 0x69, 0xFF, 0xFA, 0x24, 0x42, 0x00, 0x08,
	0x03, 0xE0, 0x00, 0x08, 0x00, 0x00, 0x10, 0x25, 0x31, 0x49, 0xFF, 0xFF,
	0x03, 0xE0, 0x00, 0x08, 0x01, 0x89, 0x18, 0x21
};

constexpr int stringHelperFindTable_TableAddrBytes[4] = { 2, 3, 6, 7 };

constexpr uint8_t stringInitInjection[200] = {
	0x27, 0xBD, 0xFF, 0xE0, 0xAF, 0xB0, 0x00, 0x10, 0xAF, 0xB1, 0x00, 0x0C,
	0x00, 0x00, 0x80, 0x25, 0x00, 0x00, 0x88, 0x25, 0xAF, 0xBF, 0x00, 0x08,
	0xAF, 0xA4, 0x00, 0x04, 0x0C, 0x00, 0x33, 0x33, 0x00, 0xA0, 0x20, 0x25,
	0x10, 0x40, 0x00, 0x09, 0x8F, 0xA4, 0x00, 0x04, 0xAF, 0xA3, 0x00, 0x18,
	0x8C, 0x48, 0x00, 0x04, 0x00, 0x05, 0x49, 0x00, 0x00, 0x89, 0x88, 0x21,
	0x8E, 0x30, 0x00, 0x14, 0x00, 0x00, 0x00, 0x00, 0x02, 0x08, 0x40, 0x21,
	0xAE, 0x28, 0x00, 0x14, 0x3C, 0x01, 0x80, 0x04, 0x34, 0x21, 0xC3, 0xEC,
	0x00, 0x20, 0xF8, 0x09, 0x00, 0x00, 0x00, 0x00, 0x12, 0x00, 0x00, 0x10,
	0x00, 0x00, 0x00, 0x00, 0xAF, 0xA2, 0x00, 0x14, 0x8E, 0x28, 0x00, 0x14,
	0x00, 0x50, 0x20, 0x21, 0x8F, 0xA5, 0x00, 0x18, 0x3C, 0x09, 0x03, 0xFF,
	0x35, 0x29, 0xFF, 0xFF, 0x00, 0xA9, 0x28, 0x24, 0x01, 0x10, 0x30, 0x23,
	0x00, 0xA6, 0x30, 0x21, 0x00, 0x00, 0x38, 0x25, 0x3C, 0x01, 0x80, 0x00,
	0x34, 0x21, 0x3F, 0x74, 0x00, 0x20, 0xF8, 0x09, 0x00, 0x00, 0x00, 0x00,
	0x8F, 0xA2, 0x00, 0x14, 0x8F, 0xB0, 0x00, 0x10, 0x8F, 0xB1, 0x00, 0x0C,
	0x8F, 0xBF, 0x00, 0x08, 0x27, 0xBD, 0x00, 0x20, 0x10, 0x40, 0x00, 0x03,
	0x00, 0x00, 0x00, 0x00, 0x3C, 0x1F, 0x80, 0x04, 0x37, 0xFF, 0xC4, 0xF0,
	0x03, 0xE0, 0x00, 0x08, 0x8F, 0xA3, 0x00, 0x1C
};


constexpr int stringInit_HelperJalBytes[4] = { 28,29,30,31};

constexpr uint8_t stringInitDelaySlot[4] = {
	0xAF, 0xA0, 0x00, 0x1C
};

constexpr uint8_t stringGetInjection[184] = {
	0x27, 0xBD, 0xFF, 0xF0, 0xAF, 0xBF, 0x00, 0x04, 0x0C, 0x00, 0x33, 0x33,
	0x00, 0x00, 0x00, 0x00, 0x8F, 0xBF, 0x00, 0x04, 0x27, 0xBD, 0x00, 0x10,
	0x14, 0x40, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x30, 0x25,
	0x00, 0xA0, 0x38, 0x25, 0x00, 0x00, 0x28, 0x25, 0x3C, 0x08, 0x80, 0x05,
	0x25, 0x08, 0xC7, 0xA0, 0x01, 0x00, 0x00, 0x08, 0x00, 0x00, 0x20, 0x25,
	0x3C, 0x08, 0x80, 0x09, 0x8D, 0x08, 0x71, 0x20, 0x8C, 0x4C, 0x00, 0x04,
	0x11, 0x00, 0xFF, 0xF5, 0x00, 0x00, 0x00, 0x00, 0x8D, 0x08, 0x00, 0x08,
	0x00, 0x04, 0x51, 0x00, 0x11, 0x00, 0xFF, 0xF1, 0x01, 0x0A, 0x40, 0x21,
	0x8D, 0x0B, 0x00, 0x18, 0x8D, 0x0A, 0x00, 0x14, 0x11, 0x60, 0xFF, 0xED,
	0x00, 0x00, 0x00, 0x00, 0x01, 0x4B, 0x40, 0x21, 0x01, 0x0C, 0x40, 0x23,
	0x01, 0x00, 0x18, 0x25, 0x8C, 0x69, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x09, 0x4C, 0x02, 0x00, 0x09, 0x52, 0x02, 0x00, 0xAA, 0x58, 0x23,
	0x05, 0x60, 0xFF, 0xE3, 0x31, 0x29, 0x00, 0xFF, 0x00, 0xA9, 0x08, 0x23,
	0x04, 0x21, 0xFF, 0xE0, 0x00, 0x0B, 0x48, 0x80, 0x00, 0x69, 0x48, 0x21,
	0x8D, 0x29, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x03, 0xE0, 0x00, 0x08,
	0x00, 0x69, 0x10, 0x21
};

constexpr int stringGet_HelperJalBytes[4] = { 8, 9, 10, 11 };

constexpr uint8_t stringGetDelaySlot[4] = {
	0x00, 0x00, 0x00, 0x00 //nop
};

///////////////

typedef InjectedCode 
		< false, true, rentalDelaySlot, 1,
		 rentalLoadInjection_TableAddrBytes, sizeof(rentalLoadInjection), rentalLoadInjection >
	InjectedRental;
	

typedef InjectedCode
		< false, true, itemDelaySlot, 19,
		 itemLoadInjection_TableAddrBytes, sizeof(itemLoadInjection), itemLoadInjection >
	InjectedItem;

typedef InjectedCode
		< false, true, faceDelaySlot, 0,
		faceLoadInjection_TableAddrBytes, sizeof(faceLoadInjection), faceLoadInjection >
	InjectedFace;

typedef InjectedCode
		< true, true, face2DelaySlot, 0,
		face2LoadInjection_TableAddrBytes, sizeof(face2LoadInjection), face2LoadInjection >
	InjectedFace2;


typedef InjectedCode
		< false, false, stringGetDelaySlot, 0,
		stringHelperFindTable_TableAddrBytes, sizeof(stringHelperFindTable), stringHelperFindTable >
	InjectedStringsHelper;

typedef InjectedCode
		< false, true, stringInitDelaySlot, 0,
		stringInit_HelperJalBytes, sizeof(stringInitInjection), stringInitInjection >
	InjectedStringsInit;

typedef InjectedCode
		< false, false, stringGetDelaySlot, 0,
		stringGet_HelperJalBytes, sizeof(stringGetInjection), stringGetInjection >
	InjectedStringsGet;

