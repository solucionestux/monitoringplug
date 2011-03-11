Name:           monitorplug
Version:        0.1
Release:        1%{?dist}
Summary:        Collection of monitoring plugins for Nagios and similar monitoring systems.

Group:          Applications/System
License:        GPL
URL:            http://svn.durchmesser.ch/trac/monitoringplug
Source0:        %{name}-%{version}.tar.gz
BuildRoot:      %{_tmppath}/%{name}-%{version}-%{release}-root-%(%{__id_u} -n)

BuildRequires:  ldns-devel
BuildRequires:  libselinux-devel
%if 0%{?rhel} <= 5
BuildRequires:	curl-devel
%endif
%if 0%{?rhel} >= 6
BuildRequires:	libcurl-devel
%endif
BuildRequires:	xmlrpc-c-devel
BuildRequires:	expat-devel
BuildRequires:	net-snmp-devel

%package curl
Summary:        Collection of curl-based monitoring plugins for Nagios and similar monitoring systems.
Group:          Applications/System
%if 0%{?rhel} <= 5
Requires:       curl
%endif
%if 0%{?rhel} >= 6
Requires:       libcurl
%endif

%package dns
Summary:        Collection of dns monitoring plugins for Nagios and similar monitoring systems.
Group:          Applications/System
Requires:	ldns

%package rhcs
Summary:        Collection of RedHat Cluster Suitmonitoring plugins for Nagios and similar monitoring systems.
Group:          Applications/System
Requires:	net-snmp-libs
Requires:	expat

%package selinux
Summary:        Collection of selinux monitoring plugins for Nagios and similar monitoring systems.
Group:          Applications/System
Requires:	libselinux

%package snmp
Summary:        Collection of snmp-based monitoring plugins for Nagios and similar monitoring systems.
Group:          Applications/System
Requires:	net-snmp-libs

%package xmlrpc
Summary:        Collection of xmlrpc monitoring plugins for Nagios and similar monitoring systems.
Group:          Applications/System
Requires:       xmlrpc-c-client

%description
Collection of monitoring plugins for Nagios and similar monitoring systems.
This package contains the base and dummy plugins which don't need any
additional libraries.

%description curl
Collection of monitoring plugins for Nagios and similar monitoring systems.
This package contains the curl based plugins.

%description dns
Collection of monitoring plugins for Nagios and similar monitoring systems.
This package contains the dns plugins which use the ldns library.

%description rhcs
Collection of monitoring plugins for Nagios and similar monitoring systems.
This package contains the RedHat Cluster Suite plugins using snmp and expat.

%description selinux
Collection of monitoring plugins for Nagios and similar monitoring systems.
This package contains the selinux plugins.

%description snmp
Collection of monitoring plugins for Nagios and similar monitoring systems.
This package contains the snmp based plugins.

%description xmlrpc
Collection of monitoring plugins for Nagios and similar monitoring systems.
This package contains the xmlrpc plugins.

%prep
%setup -q

%build
%if 0%{?rhel} <= 5
%configure SELINUX_CFLAGS=" " SELINUX_LIBS=-lselinux
%endif
%if 0%{?rhel} >= 6
%configure
%endif
make %{?_smp_mflags}

%check
make %{?_smp_mflags} check

%install
rm -rf $RPM_BUILD_ROOT
make install DESTDIR=$RPM_BUILD_ROOT

%clean
rm -rf $RPM_BUILD_ROOT

%files
%defattr(-,root,root,-)
%{_libdir}/nagios/plugins/check_file
%{_libdir}/nagios/plugins/check_sockets
%{_libdir}/nagios/plugins/check_dummy
%{_libdir}/nagios/plugins/check_timeout
%doc %{_defaultdocdir}/%{name}

%files curl
%{_libdir}/nagios/plugins/check_aspsms_credits
%{_libdir}/nagios/plugins/check_tftp

%files dns
%{_libdir}/nagios/plugins/check_dns_*
%{_libdir}/nagios/plugins/check_dnssec_*

%files rhcs
%{_libdir}/nagios/plugins/check_clustat
%{_libdir}/nagios/plugins/check_rhcsnmp

%files selinux
%{_libdir}/nagios/plugins/check_enforce
%{_libdir}/nagios/plugins/check_sebool

%files snmp
%{_libdir}/nagios/plugins/check_apc_pdu

%files xmlrpc
%{_libdir}/nagios/plugins/check_rhn_entitlements


%changelog
* Sat Feb 26 2010 Marius Rieder <marius.rieder@durchmesser.ch> - 0.1-1
- First release