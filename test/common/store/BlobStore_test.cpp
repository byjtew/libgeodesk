// Copyright (c) 2024 Clarisma / GeoDesk contributors
// SPDX-License-Identifier: LGPL-3.0-only

#include <memory>
#include <string_view>
#include <catch2/catch_test_macros.hpp>
#include "clarisma/store/BlobStore_v2.h"

using namespace clarisma;
using namespace clarisma::v2;

/*

std::unique_ptr<uint8_t[]> createJunk(size_t size)
{
	// Define the pattern "ABCDEFG..."
	const char* pattern = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	size_t patternLength = strlen(pattern);

	// Allocate the memory block
	std::unique_ptr<uint8_t[]> buffer(new uint8_t[size]);

	// Fill the buffer with the repeating pattern
	for (size_t i = 0; i < size; ++i)
	{
		buffer[i] = pattern[i % patternLength];
	}

	return buffer;
}



TEST_CASE("BlobStore")
{
	const char *filename = R"(c:\geodesk\tests\blobstore.bin)";
	BlobStore::CreateTransaction<BlobStore> t0;
	t0.begin(filename);
	t0.commit();
	t0.end();

	BlobStore store;
	store.open(filename, File::OpenMode::READ | File::OpenMode::WRITE);
	BlobStore::Transaction t1(&store);
	std::string_view dataA("Test data");
	std::string_view dataB("More test data");
	std::string_view dataC("Even more test data");
	BlobStore::PageNum a = t1.addBlob(ByteSpan(reinterpret_cast<const uint8_t*>(dataA.data()), dataA.size()));
	BlobStore::PageNum b = t1.addBlob(ByteSpan(reinterpret_cast<const uint8_t*>(dataB.data()), dataB.size()));
	t1.commit();
	t1.end();

	BlobStore::Transaction t2(&store);
    t2.begin();
	t2.free(a);
	t2.commit();
	t2.end();

	BlobStore::Transaction t3(&store);
	t3.begin();
	BlobStore::PageNum c = t3.addBlob(ByteSpan(reinterpret_cast<const uint8_t*>(dataC.data()), dataC.size()));
    t3.commit();
	t3.end();

	store.close();
}


TEST_CASE("BlobStore Hole-Punching")
{
	const char *filename = R"(c:\geodesk\tests\blobstore-holes.bin)";
	BlobStore::CreateTransaction<BlobStore> t0;
	t0.begin(filename);
	t0.commit();
	t0.end();

	BlobStore store;
	store.open(filename, File::READ | File::WRITE);
	BlobStore::Transaction t1(&store);
    t1.begin();
	size_t chunkSize = 16 * 1024 * 1024;
	std::unique_ptr<uint8_t[]> junk = createJunk(chunkSize);
	BlobStore::PageNum a = t1.addBlob(ByteSpan(junk.get(), chunkSize));
	BlobStore::PageNum b = t1.alloc(3 * 1024); 
	t1.commit();
	t1.end();

	BlobStore::Transaction t2(&store);
    t2.begin();
	t2.free(a);
	t2.commit();
	t2.end();

	store.close();
}

*/