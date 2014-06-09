%bcond_with wayland

Name:       ise-default
Summary:    Tizen keyboard
Version:    1.0.8
Release:    1
Group:      TO BE / FILLED IN
License:    TO BE / FILLED IN
Source0:    ise-default-%{version}.tar.gz
Requires(post): /sbin/ldconfig
Requires(postun): /sbin/ldconfig
BuildRequires:  gettext-tools
BuildRequires:  edje-bin
BuildRequires:  cmake
BuildRequires:  pkgconfig(elementary)
%if %{with wayland}
BuildRequires:  pkgconfig(ecore-wayland)
BuildRequires:  pkgconfig(wayland-client)
%else
BuildRequires:  pkgconfig(utilX)
BuildRequires:  pkgconfig(x11)
%endif
BuildRequires:  pkgconfig(isf)
BuildRequires:  pkgconfig(vconf)
BuildRequires:  pkgconfig(aul)
BuildRequires:  pkgconfig(dlog)
BuildRequires:  pkgconfig(libscl-ui)
BuildRequires:  pkgconfig(ecore-imf)
BuildRequires:  pkgconfig(libxml-2.0)
BuildRequires:  pkgconfig(efl-assist)



%description
Description: Tizen keyboard



%prep
%setup -q


%build
%if %{with wayland}
cmake . -DCMAKE_INSTALL_PREFIX=%{_prefix} -Dwith_wayland=TRUE
%else
cmake . -DCMAKE_INSTALL_PREFIX=%{_prefix}
%endif

make %{?jobs:-j%jobs}

%install
rm -rf %{buildroot}
mkdir -p %{buildroot}/usr/share/license
cp LICENSE.Flora %{buildroot}/usr/share/license/%{name}

%make_install

%post

%postun

%files 
%defattr(-,root,root,-)
%{_libdir}/scim-1.0/1.4.0/Helper/ise-default.so
%{_libdir}/scim-1.0/1.4.0/SetupUI/ise-default-setup.so
%{_datadir}/isf/ise/ise-default/*
%{_datadir}/packages/*
%{_datadir}/locale/*
/usr/share/license/%{name}

