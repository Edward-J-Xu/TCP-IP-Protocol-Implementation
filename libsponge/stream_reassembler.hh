#ifndef SPONGE_LIBSPONGE_STREAM_REASSEMBLER_HH
#define SPONGE_LIBSPONGE_STREAM_REASSEMBLER_HH

#include "byte_stream.hh"

#include <cstdint>
#include <string>
#include <map>
#include <set>
#include <algorithm>
#include <iostream>

class typeUnassembled{
  public:
    size_t index;
    std::string data;
    typeUnassembled(size_t _index,std::string _data):index(_index),data(_data){}
    bool operator <(const typeUnassembled& t1) const {
      return index < t1.index;
    }
};

//! \brief A class that assembles a series of excerpts from a byte stream (possibly out of order,
//! possibly overlapping) into an in-order byte stream.
class StreamReassembler {
  private:

    // Your code here -- add private members as necessary.
    ByteStream _output;  //!< The reassembled in-order byte stream
    std::set<typeUnassembled> _Unassembled;
    size_t _firstUnassembled;
    size_t _nUnassembled;
    size_t _capacity;  //!< The maximum number of bytes
    bool _eof;

  public:
    //! \brief Construct a `StreamReassembler` that will store up to `capacity` bytes.
    //! \note This capacity limits both the bytes that have been reassembled,
    //! and those that have not yet been reassembled.
    StreamReassembler(const size_t capacity);

    //! \brief Receive a substring and write any newly contiguous bytes into the stream.
    //!
    //! The StreamReassembler will stay within the memory limits of the `capacity`.
    //! Bytes that would exceed the capacity are silently discarded.
    //!
    //! \param data the substring
    //! \param index indicates the index (place in sequence) of the first byte in `data`
    //! \param eof the last byte of `data` will be the last byte in the entire stream
    void push_substring(const std::string &data, const uint64_t index, const bool eof);

    // 合并两个_Unassembled的子串，利用iter2更新index和data信息，并删除iter2
    void merge_substring(size_t &index,std::string &data,std::set<typeUnassembled>::iterator iter2);


    //! \name Access the reassembled byte stream
    //!@{
    const ByteStream &stream_out() const { return _output; }
    ByteStream &stream_out() { return _output; }
    //!@}

    //! The number of bytes in the substrings stored but not yet reassembled
    //!
    //! \note If the byte at a particular index has been pushed more than once, it
    //! should only be counted once for the purpose of this function.
    size_t unassembled_bytes() const;

    //! \brief Is the internal state empty (other than the output stream)?
    //! \returns `true` if no substrings are waiting to be assembled
    bool empty() const;
};

#endif  // SPONGE_LIBSPONGE_STREAM_REASSEMBLER_HH
