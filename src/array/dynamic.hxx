
  template <class _Ty, size_t _Size>
  class array {
  public:
      using value_type      = _Ty;
      using size_type       = size_t;
      using difference_type = ptrdiff_t;
      using pointer         = _Ty*;
      using const_pointer   = const _Ty*;
      using reference       = _Ty&;
      using const_reference = const _Ty&;

      using iterator       = _Array_iterator<_Ty, _Size>;
      using const_iterator = _Array_const_iterator<_Ty, _Size>;

      using reverse_iterator       = _STD reverse_iterator<iterator>;
      using const_reverse_iterator = _STD reverse_iterator<const_iterator>;

      void fill(const _Ty& _Value);
      void swap(array& _Other) noexcept(_Is_nothrow_swappable<_Ty>::value);

      iterator begin() noexcept;
      const_iterator begin() const noexcept;
      iterator end() noexcept;
      const_iterator end() const noexcept;
      reverse_iterator rbegin() noexcept;
      const_reverse_iterator rbegin() const noexcept;
      reverse_iterator rend() noexcept;
      const_reverse_iterator rend() const noexcept;
      const_iterator cbegin() const noexcept;
      const_iterator cend() const noexcept;
      const_reverse_iterator crbegin() const noexcept;
      const_reverse_iterator crend() const noexcept;

      constexpr size_type size() const noexcept;
      constexpr size_type max_size() const noexcept;
      constexpr bool empty() const noexcept;
      reference at(size_type _Pos);
      constexpr const_reference at(size_type _Pos) const;
      reference operator[](_In_range_(0, _Size - 1) size_type _Pos) noexcept;
      constexpr const_reference operator[](_In_range_(0, _Size - 1) size_type _Pos) const;
      reference front() noexcept;
      constexpr const_reference front() const noexcept;
      reference back() noexcept;
      constexpr const_reference back() const noexcept;
      _Ty* data() noexcept;
      const _Ty* data() const noexcept;
  };

  class vector {
    public:
      using iterator               = _Vector_iterator<_Scary_val>;
      using const_iterator         = _Vector_const_iterator<_Scary_val>;
      using reverse_iterator       = _STD reverse_iterator<iterator>;
      using const_reverse_iterator = _STD reverse_iterator<const_iterator>;

                    vector() noexcept;
          explicit  vector(const _Alloc& _Al) noexcept;
          explicit  vector(const size_type _Count, const _Alloc& _Al = _Alloc());
                    vector(const size_type _Count, const _Ty& _Val, const _Alloc& _Al = _Alloc());
                    vector(_Iter _First, _Iter _Last, const _Alloc& _Al = _Alloc());
                    vector(initializer_list<_Ty> _Ilist, const _Alloc& _Al = _Alloc());
                    vector(const vector& _Right);
                    vector(const vector& _Right, const _Identity_t<_Alloc>& _Al);
                    vector(vector&& _Right) noexcept;
                    vector(vector&& _Right, const _Identity_t<_Alloc>& _Al_) noexcept;
                   ~vector() noexcept;

            vector& operator=(vector&& _Right) noexcept;

      decltype(auto) emplace_back(_Valty&&... _Val);
                void push_back(const _Ty& _Val);
                void push_back(_Ty&& _Val);

            iterator emplace(const_iterator _Where, _Valty&&... _Val);
            iterator insert(const_iterator _Where, const _Ty& _Val);
            iterator insert(const_iterator _Where, _Ty&& _Val);
            iterator insert(const_iterator _Where, const size_type _Count, const _Ty& _Val);
            iterator insert(const_iterator _Where, _Iter _First, _Iter _Last);
            iterator insert(const_iterator _Where, initializer_list<_Ty> _Ilist);

         void assign(_CRT_GUARDOVERFLOW const size_type _Newsize, const _Ty& _Val);
         void assign(_Iter _First, _Iter _Last) {
            _Adl_verify_range(_First, _Last);
            auto _UFirst = _Get_unwrapped(_First);
            auto _ULast  = _Get_unwrapped(_Last);
            if constexpr (_Is_cpp17_fwd_iter_v<_Iter>) {
                const auto _Length = static_cast<size_t>(_STD distance(_UFirst, _ULast));
                const auto _Count  = _Convert_size<size_type>(_Length);
                _Assign_counted_range(_UFirst, _Count);
    #ifdef __cpp_lib_concepts
            } else if constexpr (forward_iterator<_Iter>) {
                const auto _Length = _To_unsigned_like(_RANGES distance(_UFirst, _ULast));
                const auto _Count  = _Convert_size<size_type>(_Length);
                _Assign_counted_range(_UFirst, _Count);
    #endif // __cpp_lib_concepts
            } else {
                _Assign_uncounted_range(_UFirst, _ULast);
            }
        }
         void assign(const initializer_list<_Ty> _Ilist) {
            const auto _Count = _Convert_size<size_type>(_Ilist.size());
            _Assign_counted_range(_Ilist.begin(), _Count);
        }
         vector& operator=(const vector& _Right) {
            if (this == _STD addressof(_Right)) {
                return *this;
            }

            auto& _Al       = _Getal();
            auto& _Right_al = _Right._Getal();
            if constexpr (_Choose_pocca_v<_Alty>) {
                if (_Al != _Right_al) {
                    _Tidy();
                    _Mypair._Myval2._Reload_proxy(_GET_PROXY_ALLOCATOR(_Alty, _Al), _GET_PROXY_ALLOCATOR(_Alty, _Right_al));
                }
            }

            _Pocca(_Al, _Right_al);
            auto& _Right_data = _Right._Mypair._Myval2;
            _Assign_counted_range(_Right_data._Myfirst, static_cast<size_type>(_Right_data._Mylast - _Right_data._Myfirst));

            return *this;
        }
         vector& operator=(initializer_list<_Ty> _Ilist) {
            const auto _Count = _Convert_size<size_type>(_Ilist.size());
            _Assign_counted_range(_Ilist.begin(), _Count);
            return *this;
        }
         void resize(_CRT_GUARDOVERFLOW const size_type _Newsize) {
            // trim or append value-initialized elements, provide strong guarantee
            _Resize(_Newsize, _Value_init_tag{});
        }
         void resize(_CRT_GUARDOVERFLOW const size_type _Newsize, const _Ty& _Val) {
            // trim or append copies of _Val, provide strong guarantee
            _Resize(_Newsize, _Val);
        }

         void reserve(_CRT_GUARDOVERFLOW const size_type _Newcapacity);
         void pop_back() noexcept;
         iterator erase(const_iterator _Where) noexcept;
         iterator erase(const_iterator _First, const_iterator _Last) noexcept;
         void clear() noexcept;
         void swap(vector& _Right) noexcept;

      _Ty* data() noexcept;
      const _Ty* data() const noexcept;
      iterator begin() noexcept;
      const_iterator begin() const noexcept;
      iterator end() noexcept;
      const_iterator end() const noexcept;
      reverse_iterator rbegin() noexcept;
      const_reverse_iterator rbegin() const noexcept;
      reverse_iterator rend() noexcept;
      const_reverse_iterator rend() const noexcept;
      const_iterator cbegin() const noexcept;
      const_iterator cend() const noexcept;
      const_reverse_iterator crbegin() const noexcept;
      const_reverse_iterator crend() const noexcept;
      pointer _Unchecked_begin() noexcept;
      const_pointer _Unchecked_begin() const noexcept;
      pointer _Unchecked_end() noexcept;
      const_pointer _Unchecked_end() const noexcept;
      bool empty() const noexcept;
      size_type size() const noexcept;
      size_type max_size() const noexcept;
      size_type capacity() const noexcept;
      _Ty& operator[](const size_type _Pos) noexcept;
      const _Ty& operator[](const size_type _Pos) const noexcept;
      _Ty& at(const size_type _Pos);
      const _Ty& at(const size_type _Pos) const;
      _Ty& front() noexcept;
      const _Ty& front() const noexcept;
      _Ty& back() noexcept;
      const _Ty& back() const noexcept;
      allocator_type get_allocator() const noexcept;

  };