    subroutine MPP_GLOBAL_FIELD_2D_( domain, local, global, flags, new, dc_handle, position,tile_count)
      type(domain2D), intent(in) :: domain
      MPP_TYPE_, intent(in)  ::  local(:,:)
      MPP_TYPE_, intent(out) :: global(:,:)
      integer, intent(in), optional :: flags
      logical, intent(in), optional :: new
      integer, intent(in), optional :: position
      integer, intent(in), optional :: tile_count

      MPP_TYPE_ :: local3D (size( local,1),size( local,2),1)
      MPP_TYPE_ :: global3D(size(global,1),size(global,2),1)

#ifdef use_CRI_pointers
      type(DomainCommunicator2D),pointer,optional :: dc_handle
      pointer( lptr,  local3D )
      pointer( gptr, global3D )
      lptr = LOC( local)
      gptr = LOC(global)
      call mpp_global_field( domain, local3D, global3D, flags, new, dc_handle, position,tile_count )
#else
      integer, optional :: dc_handle  ! not used when there are no cray pointers
      local3D = RESHAPE( local, SHAPE(local3D) )
      call mpp_global_field( domain, local3D, global3D, flags, position=position )
      global = RESHAPE( global3D, SHAPE(global) )
#endif
    end subroutine MPP_GLOBAL_FIELD_2D_

    subroutine MPP_GLOBAL_FIELD_3D_( domain, local, global, flags, new, dc_handle, position, tile_count)
!get a global field from a local field
!local field may be on compute OR data domain
      type(domain2D), intent(in) :: domain
      MPP_TYPE_, intent(in)  ::  local(:,:,:)
      MPP_TYPE_, intent(out) :: global(:,:,:)
      integer, intent(in), optional :: flags
      logical, intent(in), optional :: new
      integer, intent(in), optional :: position
      integer, intent(in), optional :: tile_count

#ifdef use_CRI_pointers
      type(DomainCommunicator2D),pointer,optional :: dc_handle
      type(DomainCommunicator2D),pointer,save :: d_comm =>NULL()
      integer(LONG_KIND)     :: l_addr
      logical                :: use_new
      integer :: isize_l,jsize_l,isize_g,jsize_g,ke, tile
      type(domain2d), pointer :: Dom => NULL()

      Dom => get_domain( domain, position )
      use_new=.false.; if(PRESENT(new))use_new=new
      tile = 1; if(PRESENT(tile_count)) tile = tile_count

      if(use_new)then
        if(domain%ntiles>1) call mpp_error(FATAL,"MPP_GLOBAL_FIELD: new version mpp_global_field for multple-tile mosaic "// &
                                          "is not implemented yet, contact developer")
        if(PRESENT(flags)) then
           if(BTEST(flags, ROOT_GLOBAL)) call mpp_error(FATAL,"MPP_GLOBAL_FIELD: global_field_on_root_pe_only capability "// &
               "is not implemented in new version mpp_global_field, contact developer")
        endif

        ke = size(local,3)
        if(ke /= size(global,3))call mpp_error(FATAL, 'MPP_GLOBAL_FIELD: incoming arrays have mismatched K extent.')
        isize_l = size(local,1); jsize_l = size(local,2)
        isize_g = size(global,1); jsize_g = size(global,2)

        l_addr = LOC(local)
        if(PRESENT(dc_handle))d_comm =>dc_handle  ! User has kept pointer to d_comm
        if(.not.ASSOCIATED(d_comm))then  ! d_comm needs initialization or lookup
          d_comm =>mpp_global_field_init_comm(Dom,l_addr,isize_g,jsize_g,isize_l,jsize_l,ke, &
                                              flags=flags, position=position)
          if(PRESENT(dc_handle))dc_handle =>d_comm  ! User wants to keep pointer to d_comm
        endif
        call mpp_do_global_field( local, global, d_comm )
        d_comm =>NULL()
      else
        call mpp_do_global_field( Dom, local, global, tile, flags)
      end if
      Dom =>NULL()
#else
      integer, optional :: dc_handle  ! not used when there are no cray pointers
      type(domain2d), pointer :: Dom => NULL()

      Dom => get_domain( domain, position )
      tile = 1; if(PRESENT(tile_count)) tile = tile_count
      call mpp_do_global_field( Dom, local, global, tile, flags )
      Dom =>NULL()
#endif
    end subroutine MPP_GLOBAL_FIELD_3D_

    subroutine MPP_GLOBAL_FIELD_4D_( domain, local, global, flags, new, dc_handle, position,tile_count )
      type(domain2D), intent(in) :: domain
      MPP_TYPE_, intent(in)  ::  local(:,:,:,:)
      MPP_TYPE_, intent(out) :: global(:,:,:,:)
      integer, intent(in), optional :: flags
      logical, intent(in), optional :: new
      integer, intent(in), optional :: position
      integer, intent(in), optional :: tile_count

      MPP_TYPE_ :: local3D (size( local,1),size( local,2),size( local,3)*size(local,4))
      MPP_TYPE_ :: global3D(size(global,1),size(global,2),size(global,3)*size(local,4))
#ifdef use_CRI_pointers
      type(DomainCommunicator2D),pointer,optional :: dc_handle
      pointer( lptr, local3D  )
      pointer( gptr, global3D )
      lptr = LOC(local)
      gptr = LOC(global)
      call mpp_global_field( domain, local3D, global3D, flags, new, dc_handle, position,tile_count )
#else
      integer, optional :: dc_handle  ! not used when there are no cray pointers
      local3D = RESHAPE( local, SHAPE(local3D) )
      call mpp_global_field( domain, local3D, global3D, flags, position = position  )
      global = RESHAPE( global3D, SHAPE(global) )
#endif
    end subroutine MPP_GLOBAL_FIELD_4D_

    subroutine MPP_GLOBAL_FIELD_5D_( domain, local, global, flags, new, dc_handle, position,tile_count )
      type(domain2D), intent(in) :: domain
      MPP_TYPE_, intent(in)  ::  local(:,:,:,:,:)
      MPP_TYPE_, intent(out) :: global(:,:,:,:,:)
      integer, intent(in), optional :: flags
      logical, intent(in), optional :: new
      integer, intent(in), optional :: position
      integer, intent(in), optional :: tile_count

      MPP_TYPE_ :: local3D (size( local,1),size( local,2),size( local,3)*size( local,4)*size(local,5))
      MPP_TYPE_ :: global3D(size(global,1),size(global,2),size(global,3)*size(global,4)*size(local,5))
#ifdef use_CRI_pointers
      type(DomainCommunicator2D),pointer,optional :: dc_handle
      pointer( lptr, local3D  )
      pointer( gptr, global3D )
      lptr = LOC(local)
      gptr = LOC(global)
      call mpp_global_field( domain, local3D, global3D, flags, new, dc_handle, position,tile_count )
#else
      integer, optional :: dc_handle  ! not used when there are no cray pointers
      local3D = RESHAPE( local, SHAPE(local3D) )
      call mpp_global_field( domain, local3D, global3D, flags, position = position )
      global = RESHAPE( global3D, SHAPE(global) )
#endif
    end subroutine MPP_GLOBAL_FIELD_5D_