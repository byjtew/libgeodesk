// Copyright (c) 2024 Clarisma / GeoDesk contributors
// SPDX-License-Identifier: LGPL-3.0-only

#pragma once

#include <clarisma/store/Store.h>
#include <unordered_map>
#include <clarisma/util/DataPtr.h>
#include <clarisma/util/pointer.h>

namespace clarisma {

class BlobStore : public Store
{
public:
	void prefetchBlob(void* pBlob)
	{
		uint32_t size = pointer(pBlob).getUnsignedInt() & 0x3fff'ffff;
		prefetch(pBlob, size);
	}

	using PageNum = uint32_t;

	struct Header
	{
		uint32_t magic;
		uint32_t version;
		uint64_t creationTimestamp;
		uint32_t totalPageCount;
		
		/*
		uint8_t  guid[16];
		uint8_t  pageSize;
		uint32_t reserved : 24;
		uint32_t metadataSize;
		uint32_t propertiesPointer;
		uint32_t indexPointer;

		/// The total number of pages in use (includes free blobs and metadata pages).
		///
		uint32_t totalPageCount;

		/// A bitfield indicating which spans of 16 slots in the Trunk Free-Table
		/// have at least one slot that is non-zero.
		///
		uint32_t trunkFreeTableRanges;
		uint32_t datasetVersion;
		uint32_t reserved2;
		uint8_t  subtypeData[64];
		*/

		/**
		 * The Trunk Free-Table
		 */
		// PageNum trunkFreeTable[512];
	};

	//struct Header
	//{
	//	uint32_t magic;
	//	uint16_t versionLow;
	//	uint16_t versionHigh;
	//	DateTime creationTimestamp;
	//	// uint8_t  guid[16];
	//	uint8_t  pageSize;
	//	uint8_t  reserved[3];
	//	uint32_t metadataSize;
	//	uint32_t propertiesPointer;
	//	// uint32_t indexPointer;

	//	/**
	//	 * The total number of pages in use (includes free blobs and metadata pages).
	//	 */
	//	uint32_t totalPageCount;

	//	/**
	//	 * A bitfield indicating which spans of 16 slots in the Trunk Free-Table
	//	 * have at least one slot that is non-zero.
	//	 */
	//	uint32_t trunkFreeTableRanges;

	//	// uint32_t datasetVersion;
	//	uint8_t reserved2[28];
	//	uint8_t  subtypeData[64];

	//	/**
	//	 * The Trunk Free-Table
	//	 */
	//	PageNum trunkFreeTable[512];
	//};
	// static_assert(offsetof(Header, subtypeData) == 64, "Expected 64-byte Header");

	struct Blob
	{
		uint32_t precedingFreeBlobPages;
		uint32_t payloadSize : 30;
		unsigned unused : 1;			// TODO: bool causes alignment issues on MSVC?
		unsigned isFree : 1;
		PageNum  prevFreeBlob;
		PageNum  nextFreeBlob;
		uint32_t leafFreeTableRanges;
		uint8_t  unused2[44];
		PageNum  leafFreeTable[512];
	};

	class Transaction : private Store::Transaction
	{
	public:
		explicit Transaction(BlobStore* store) :
			Store::Transaction(store)
		{
		}

		BlobStore* store() const { return reinterpret_cast<BlobStore*>(store_); }
		PageNum alloc(uint32_t payloadSize);
		void free(PageNum firstPage);
		void commit();

	private:
		Header* getRootBlock() 
		{ 
			return reinterpret_cast<Header*>(getBlock(0)); 
		}

		Blob* getBlobBlock(PageNum page)
		{
			return reinterpret_cast<Blob*>(
				getBlock(static_cast<uint64_t>(page) 
					<< store()->pageSizeShift_));
		}

		void addFreeBlob(PageNum firstPage, uint32_t pages, uint32_t precedingFreePages);
		void removeFreeBlob(Blob* freeBlock);
		PageNum relocateFreeTable(PageNum page, int sizeInPages);
		bool isFirstPageOfSegment(PageNum page) const
		{
			return (page & ((0x3fff'ffff) >> store()->pageSizeShift_)) == 0;
		}

		std::unordered_map<PageNum, uint32_t> freedBlobs_;
	};

	uint8_t* translatePage(uint32_t page);

protected:
	static const uint32_t MAGIC = 0x7ADA0BB1;
	static const uint32_t VERSION = 1'000'000;

	static const int BLOB_HEADER_SIZE = 8;

	static const int BLOB_PAYLOAD_SIZE_OFS = 4;

	/**
	 * A bit mask that, when applied to a Blob's header word, yields
	 * the size of the Blob's payload (max. 1 GB - 8).
	 */
	// static const uint32_t PAYLOAD_SIZE_MASK = 0x3fff'ffff;

	/**
	 * Flag to indicate that a Blob is free. Stored in the payload length
	 * field of the Blob Header
	 */
	// static const uint32_t FREE_BLOB_FLAG = 0x8000'0000;

	// static const int FREE_TABLE_LEN = 2048;
	
	/*
	static const int VERSION_OFS = 4;
	static const int LOCAL_CREATION_TIMESTAMP_OFS = 8;
	static const int TOTAL_PAGES_OFS = 16;		// TODO: will change
	static const int TRUNK_FT_RANGE_BITS_OFS = 52;
	*/

	/**
	 * Offset of the trunk free-table.
	 * (This offset must be evenly divisible by 64)
	 */
	// static const int TRUNK_FREE_TABLE_OFS = 128;     // must be divisible by 64

	/*
	static const int PREV_FREE_BLOB_OFS = 8;
	static const int NEXT_FREE_BLOB_OFS = 12;
	static const int LEAF_FT_RANGE_BITS_OFS = 16;
	static const int LEAF_FREE_TABLE_OFS = 64;     // must be divisible by 64
	*/

	// TODO: Return BlobPtr
	DataPtr pagePointer(PageNum page)
	{
		return DataPtr(data(static_cast<uint64_t>(page) << pageSizeShift_));
	}

	void createStore() override;
	void verifyHeader() const override;
	void initialize() override;

	const Header* getRoot() const
	{
		return reinterpret_cast<Header*>(mainMapping());
	}

	uint64_t getLocalCreationTimestamp() const override;
	uint64_t getTrueSize() const override;
	uint32_t pagesForPayloadSize(uint32_t payloadSize) const;
	
		
private:
	uint32_t pageSizeShift_ = 12;	// TODO: default 4KB page
};



} // namespace clarisma
