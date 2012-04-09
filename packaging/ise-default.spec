Name:       ise-default
Summary:    Tizen keyboard
Version:    0.2.2
Release:    61
Group:      TO_BE/FILLED_IN
License:    Samsung Proprietary License
Source0:    %{name}-%{version}.tar.gz
Requires(post): /sbin/ldconfig, /bin/ln
Requires(postun): /sbin/ldconfig
BuildRequires: libgcrypt-devel
BuildRequires: gettext-tools
BuildRequires: pkgconfig(elementary)
BuildRequires: pkgconfig(utilX)
BuildRequires: pkgconfig(isf)
BuildRequires: pkgconfig(vconf)
BuildRequires: pkgconfig(heynoti)
BuildRequires: pkgconfig(aul)
BuildRequires: pkgconfig(ui-gadget)

%description
Description: Tizen keyboard


%prep
%setup -q

%build
./bootstrap
PREFIX="%{_prefix}"; export PREFIX
%configure --disable-static --prefix=%{_prefix}

make %{?jobs:-j%jobs}


%install
rm -rf %{buildroot}
%make_install

%files
%defattr(-,root,root,-) 
%{_libdir}/scim-1.0/1.4.0/*
%{_datadir}/isf/ise/ise-default/*
%{_datadir}/scim/icons/*
%{_datadir}/locale/*
