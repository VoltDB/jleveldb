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

public class NativeInterface {

    public NativeInterface() {
        System.loadLibrary("jleveldb");
    }

    native long leveldb_open(long options, String name);
    native void leveldb_close(long db);
    native void leveldb_put(long db, long options, byte[] key, byte[] value);
    native void leveldb_delete(long db, long options, byte[] key);
    native void leveldb_write(long db, long options, long batch);
    native byte[] leveldb_get(long db, long options, byte[] key);

    native long leveldb_create_iterator(long db, long options);
    native long leveldb_create_snapshot(long db);
    native void leveldb_release_snapshot(long db, long snapshot);
    native String leveldb_property_value(long db, String propname);

    /* Management operations */

    native void leveldb_destroy_db(long options, String name);
    native void leveldb_repair_db(long options, String name);

    /* Iterator */

    native void leveldb_iter_destroy(long iterator);
    native boolean leveldb_iter_valid(long iterator);
    native void leveldb_iter_seek_to_first(long iterator);
    native void leveldb_iter_seek_to_last(long iterator);
    native void leveldb_iter_seek(long iterator, String k, long klen);
    native void leveldb_iter_next(long iterator);
    native void leveldb_iter_prev(long iterator);
    native byte[] leveldb_iter_key(long iterator);
    native byte[] leveldb_iter_value(long iterator);
    native String leveldb_iter_get_error(long iterator);

    /* Write batch */

    native long leveldb_writebatch_create();
    native void leveldb_writebatch_destroy(long writebatch);
    native void leveldb_writebatch_clear(long writebatch);
    native void leveldb_writebatch_put(long writebatch, byte[] key, byte[] val);
    native void leveldb_writebatch_delete(long writebatch, byte[] key);

    /* Options */

    native long leveldb_options_create();
    native void leveldb_options_destroy(long options);
    native void leveldb_options_set_create_if_missing(long options, boolean value);
    native void leveldb_options_set_error_if_exists(long options, boolean value);
    native void leveldb_options_set_paranoid_checks(long options, boolean value);
    native void leveldb_options_set_env(long options, long env);
    native void leveldb_options_set_info_log(long options, long log);
    native void leveldb_options_set_write_buffer_size(long options, long size);
    native void leveldb_options_set_max_open_files(long options, int size);
    native void leveldb_options_set_cache(long options, long cache);
    native void leveldb_options_set_block_size(long options, long size);
    native void leveldb_options_set_block_restart_interval(long options, int interval);

    static final int leveldb_no_compression = 0;
    static final int leveldb_snappy_compression = 1;
    native void leveldb_options_set_compression(long options, int compression_level);

    /* Read options */

    native long leveldb_readoptions_create();
    native void leveldb_readoptions_destroy(long options);
    native void leveldb_readoptions_set_verify_checksums(long options, boolean value);
    native void leveldb_readoptions_set_fill_cache(long options, boolean value);
    native void leveldb_readoptions_set_snapshot(long options, long snapshot);

    /* Write options */

    native long leveldb_writeoptions_create();
    native void leveldb_writeoptions_destroy(long writeoptions);
    native void leveldb_writeoptions_set_sync(long writeoptions, boolean value);

    /* Cache */

    native long leveldb_cache_create_lru(long capacity);
    native void leveldb_cache_destroy(long cache);

    /* Env */

    native long leveldb_create_default_env();
    native void leveldb_env_destroy(long env);
}
