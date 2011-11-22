/* Copyright (C) 2008-2011 VoltDB Inc.
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
 * OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */

package org.voltdb.leveldb;

import java.util.Arrays;

import junit.framework.TestCase;

public class TestNative extends TestCase {

    public void testBasics() {
        NativeInterface ni = new NativeInterface();

        long options = ni.leveldb_options_create();
        ni.leveldb_options_set_create_if_missing(options, true);
        long writeoptions = ni.leveldb_writeoptions_create();
        long readoptions = ni.leveldb_readoptions_create();

        try {
            ni.leveldb_open(options, null);
            fail(); // basic null handling check
        }
        catch (RuntimeException e) {}

        long db = ni.leveldb_open(options, "testfile.leveldb");
        ni.leveldb_put(db, writeoptions, "key".getBytes(), "value".getBytes());
        byte[] value = ni.leveldb_get(db, readoptions, "key".getBytes());
        assertTrue(Arrays.equals("value".getBytes(), value));
        ni.leveldb_close(db);
        ni.leveldb_destroy_db(options, "testfile.leveldb");

        ni.leveldb_options_destroy(options);
        ni.leveldb_readoptions_destroy(readoptions);
        ni.leveldb_writeoptions_destroy(writeoptions);
    }

    public void testInsertsUpdatesDeletesAndIters() {
        NativeInterface ni = new NativeInterface();

        long options = ni.leveldb_options_create();
        ni.leveldb_options_set_create_if_missing(options, true);
        long writeoptions = ni.leveldb_writeoptions_create();
        long readoptions = ni.leveldb_readoptions_create();

        long db = ni.leveldb_open(options, "testfile.leveldb");

        for (int i = 0; i < 1000; i++) {
            byte[] key = String.valueOf(i).getBytes();
            byte[] value = ("the number " + String.valueOf(i)).getBytes();
            ni.leveldb_put(db, writeoptions, key, value);
        }

        for (int i = 1; i < 1000; i += 2) {
            byte[] key = String.valueOf(i).getBytes();
            byte[] value = ni.leveldb_get(db, readoptions, key);
            String valueStr = new String(value);
            value = (valueStr + " odd").getBytes();
            ni.leveldb_put(db, writeoptions, key, value);
        }

        for (int i = 0; i < 1000; i += 3) {
            byte[] key = String.valueOf(i).getBytes();
            boolean odd = (i % 2) == 1;
            String value = new String(ni.leveldb_get(db, readoptions, key));
            assert (odd == value.contains("odd"));
        }

        long iter = ni.leveldb_create_iterator(db, readoptions);
        for (; ni.leveldb_iter_valid(iter); ni.leveldb_iter_next(iter)) {
            String key = new String(ni.leveldb_iter_key(iter));
            int keyVal = Integer.parseInt(key);
            boolean odd = keyVal % 2 == 0;
            String value = new String(ni.leveldb_iter_value(iter));
            assert (odd == value.contains("odd"));
        }
        ni.leveldb_iter_destroy(iter);

        ni.leveldb_close(db);
        ni.leveldb_destroy_db(options, "testfile.leveldb");

        ni.leveldb_options_destroy(options);
        ni.leveldb_readoptions_destroy(readoptions);
        ni.leveldb_writeoptions_destroy(writeoptions);
    }

}
