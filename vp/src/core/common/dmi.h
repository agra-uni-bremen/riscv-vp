#pragma once

#include <stdint.h>

/*spandan--------------------*/
#define BYTES_PER_TAG 8
#define TAG_BITS_PER_ROOT 128
/*spandan--------------------*/

class MemoryDMI {
	uint8_t *mem;
	uint64_t start;
	uint64_t size;
	uint64_t offset;	//spandan
	uint64_t end;

/*spandan-------------------------------------------------------------------------------------------*/
	MemoryDMI(uint8_t *mem, uint64_t start, uint64_t size) : mem(mem), start(start), size(size), \
	offset( (size/(8*BYTES_PER_TAG) + size/(8*BYTES_PER_TAG*TAG_BITS_PER_ROOT)) ), end(start+(size-offset)) {}
/*spandan-------------------------------------------------------------------------------------------*/

   public:
	static MemoryDMI create_start_end_mapping(uint8_t *mem, uint64_t start, uint64_t end) {
		assert(end > start);
		return create_start_size_mapping(mem, start, end - start);
	}

	static MemoryDMI create_start_size_mapping(uint8_t *mem, uint64_t start, uint64_t size) {
		assert( start + size > start );
		return MemoryDMI(mem, start, size);
	}

	uint8_t *get_raw_mem_ptr() {
		return mem;
	}

/*spandan-----------------------------*/
	bool get_tag(uint64_t addr) {
		uint64_t root_bit_addr = (addr - start)/( (BYTES_PER_TAG) * (TAG_BITS_PER_ROOT) );
		uint64_t root_byte_addr = root_bit_addr/8;
		uint8_t root_byte_offset = root_bit_addr%8;
		uint8_t root_bit_mask = (0b1 << root_byte_offset);
		if (*(mem + (size-offset) + root_byte_addr) & root_bit_mask) 
		//if root bit is 1 check individual tag bits
		{
			uint64_t root_offset = size/( 8 * (BYTES_PER_TAG) * (TAG_BITS_PER_ROOT) );
			uint64_t tag_bit_addr = (addr - start)/(BYTES_PER_TAG);
			uint64_t tag_byte_addr = tag_bit_addr/8;
			uint8_t tag_byte_offset = tag_bit_addr%8;
			uint8_t tag_bit_mask = (0b1 << tag_byte_offset);
			if (*(mem + (size-offset) + (tag_byte_addr + root_offset)) & tag_bit_mask)
				return true;
			else return false;
		}
		else return false;
	}

	void set_tag(uint64_t addr, bool tag) {
		uint64_t root_bit_addr = (addr - start)/( (BYTES_PER_TAG) * (TAG_BITS_PER_ROOT) );
		uint64_t root_byte_addr = root_bit_addr/8;
		uint8_t root_byte_offset = root_bit_addr%8;
		uint8_t root_bit_mask = (0b1 << root_byte_offset);

		uint64_t root_offset = size/( 8 * (BYTES_PER_TAG) * (TAG_BITS_PER_ROOT) );
		uint64_t tag_bit_addr = (addr - start)/(BYTES_PER_TAG);
		uint64_t tag_byte_addr = tag_bit_addr/8;
		uint8_t tag_byte_offset = tag_bit_addr%8;

		uint8_t temp;
		memcpy(&temp,(mem + (size-offset) + root_byte_addr),sizeof(uint8_t));
		if (temp & root_bit_mask) //if root bit 1
		{
			if (tag) { //if tag also 1 just set tag
				uint8_t tag_bit_mask = (0b1 << tag_byte_offset);
				(*(mem + (size-offset) + (tag_byte_addr + root_offset))) |= tag_bit_mask;
			}
			else { //if tag is 0 check if all tags in the range are 0 and set root
				uint8_t tag_bit_mask = ~(0b1 << tag_byte_offset);
				(*(mem + (size-offset) + (tag_byte_addr + root_offset))) &= tag_bit_mask;

				bool val = true;
				uint64_t tag_byte_range_start = root_bit_addr * ((TAG_BITS_PER_ROOT)/8);
				uint64_t tag_byte_range_end = tag_byte_range_start + ((TAG_BITS_PER_ROOT)/8);
				for (uint64_t i = tag_byte_range_start; i < tag_byte_range_end; i++) {
					if (*(mem + (size-offset) + (i + root_offset))) {
						val = false;
						break;
					}
				}
				if (val) (*(mem + (size-offset) + root_byte_addr)) &= (~(root_bit_mask));
			}
		}
		else //if root bit is 0
		{
			if (tag) { //if tag is 1, set root 1
				uint8_t tag_bit_mask = (0b1 << tag_byte_offset);
				(*(mem + (size-offset) + (tag_byte_addr + root_offset))) |= tag_bit_mask;
				(*(mem + (size-offset) + root_byte_addr)) |= root_bit_mask;
			}
		}
	}
/*spandan-----------------------------*/

	template <typename T>
	T *get_mem_ptr_to_global_addr(uint64_t addr) {
		assert(contains(addr));
		assert((addr + sizeof(T)) <= end); //spandan
		// assert ((addr % sizeof(T)) == 0 && "unaligned access");   //NOTE: due to compressed instructions, fetching
		// can be unaligned
		return reinterpret_cast<T *>(mem + (addr - start)); //spandan: actual addr is offset by tag space
	}

/*spandan------------------------------------------------*/
	template <typename T>
	T load_tags(uint64_t addr) {
		uint64_t end_addr = addr + (BYTES_PER_TAG)*(8*sizeof(T) - 1);
		assert(contains(addr));
		assert((addr + 2*sizeof(T)) <= end); 
		if ( is_cap_not_aligned(end_addr) )
			throw std::runtime_error("Alignment error at address " + std::to_string(addr));

		uint64_t root_offset = size/( 8 * (BYTES_PER_TAG) * (TAG_BITS_PER_ROOT) );
		uint64_t tag_bit_addr = (addr - start) / (BYTES_PER_TAG);
		uint64_t tag_byte_addr = tag_bit_addr/8;
		uint8_t tag_byte_offset = tag_bit_addr%8;

		uint8_t copy_size = sizeof(T) + 1;
		T mask = ~0;

		static_assert(std::is_integral<T>::value, "integer type required");
		uint64_t ans = 0;
		T *src = reinterpret_cast<T *>(mem + (size-offset) + (tag_byte_addr + root_offset));
		// use memcpy to avoid problems with unaligned loads into standard C++ data types
		// see: https://blog.quarkslab.com/unaligned-accesses-in-cc-what-why-and-solutions-to-do-it-properly.html
		memcpy(&ans, src, copy_size);

		T final_ans = static_cast<T>((ans >> tag_byte_offset) & mask);
		return final_ans;
	}

	template <typename T>
	T load_capability(uint64_t addr) {
		if ( is_cap_not_aligned(addr) )
			throw std::runtime_error("Alignment error at address " + std::to_string(addr));

		//T is a capability, no longer of integral type
		//static_assert(std::is_integral<T>::value, "integer type required"); 
		T ans;
		uint8_t field_offset = 0;
		uint8_t *src = reinterpret_cast<uint8_t *>(get_mem_ptr_to_global_addr<T>(addr)); //The actual addr is offset by tag space
		// use memcpy to avoid problems with unaligned loads into standard C++ data types
		// see: https://blog.quarkslab.com/unaligned-accesses-in-cc-what-why-and-solutions-to-do-it-properly.html
		memcpy(&(ans.addr), (src + field_offset), sizeof(ans.addr));
		field_offset += sizeof(ans.addr);
		memcpy(&(ans.caps), (src + field_offset), sizeof(ans.caps));
		ans.tag = get_tag(addr);

		return ans;
	}

	template <typename T>
	void store_capability(uint64_t addr, T cs) {
		//Not sure if this is needed---------------------------
		if ( is_cap_not_aligned(addr) )
			throw std::runtime_error("Alignment error at address " + std::to_string(addr));
		//Not sure if this is needed---------------------------

		uint8_t *dst = reinterpret_cast<uint8_t *>(get_mem_ptr_to_global_addr<T>(addr));
		uint8_t field_offset = 0;

		memcpy( (dst+field_offset), &(cs.addr), sizeof(cs.addr) );
		field_offset += sizeof(cs.addr);
		memcpy( (dst+field_offset), &(cs.caps), sizeof(cs.caps) );
		
		set_tag(addr, cs.tag);
	}
/*spandan------------------------------------------------*/

	template <typename T>
	T load(uint64_t addr) {
		static_assert(std::is_integral<T>::value, "integer type required");
		T ans;
		T *src = get_mem_ptr_to_global_addr<T>(addr);
		// use memcpy to avoid problems with unaligned loads into standard C++ data types
		// see: https://blog.quarkslab.com/unaligned-accesses-in-cc-what-why-and-solutions-to-do-it-properly.html
		memcpy(&ans, src, sizeof(T));
		return ans;
	}

	template <typename T>
	void store(uint64_t addr, T value) {
		static_assert(std::is_integral<T>::value, "integer type required");

		set_tag(addr, false); //spandan: when data is stored, tag is set to 0 (needed?)
		T *dst = get_mem_ptr_to_global_addr<T>(addr);
		memcpy(dst, &value, sizeof(value));
	}

	uint64_t get_start() {
		return start;
	}

	uint64_t get_end() {
		return end; //spandan
	}

	uint64_t get_size() {
		return size;
	}

	bool contains(uint64_t addr) {
		return (addr >= start) && (addr < end); //spandan
	}

/*spandan-----------------------------*/
	bool is_cap_not_aligned(uint64_t addr) {
		return ((addr - start) % BYTES_PER_TAG);
	}
/*spandan-----------------------------*/
};