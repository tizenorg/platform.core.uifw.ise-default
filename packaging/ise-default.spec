Name:       ise-default
Summary:    Tizen keyboard
Version:    0.3.027
Release:    1
Group:      TO_BE/FILLED_IN
License:    Flora Software License
Source0:    %{name}-%{version}.tar.gz
Source1001: packaging/ise-default.manifest
Requires(post): /sbin/ldconfig, /bin/ln
Requires(postun): /sbin/ldconfig
BuildRequires: libgcrypt-devel
BuildRequires: gettext-tools
BuildRequires: pkgconfig(elementary)
BuildRequires: pkgconfig(utilX)
BuildRequires: pkgconfig(isf)
BuildRequires: pkgconfig(vconf)
BuildRequires: pkgconfig(heynoti)
BuildRequires: pkgconfig(sensor)
BuildRequires: pkgconfig(x11)

%description
Description: Tizen keyboard


%prep
%setup -q

%build
cp %{SOURCE1001} .
./bootstrap
PREFIX="%{_prefix}"; export PREFIX
%configure --disable-static --prefix=%{_prefix}

make %{?jobs:-j%jobs}


%install
rm -rf %{buildroot}
%make_install

%files
%manifest ise-default.manifest
%defattr(-,root,root,-) 
%{_libdir}/scim-1.0/1.4.0/*
%{_datadir}/isf/ise/ise-default/*
%{_datadir}/scim/icons/*
%{_datadir}/locale/*
