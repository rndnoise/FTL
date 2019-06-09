Name:           pihole-ftl
Version:        4.3.1
Release:        1
Summary:        The Pi-hole FTL engine, which provides DNS services

License:        EUPL-1.2
URL:            https://pi-hole.net

BuildRequires:  systemd
Requires:       libcap
%{?systemd_requires}


%description


%prep
cp -r %{_sourcedir}/* %{_builddir}


%install
rm -rf $RPM_BUILD_ROOT

# Create directories
install -d -m 0755 %{buildroot}%{_bindir}
install -d -m 0755 %{buildroot}%{_unitdir}
install -d -m 0755 %{buildroot}/etc/pihole

# Install files
install -m 0755 pihole-FTL %{buildroot}%{_bindir}
install -m 644 debian/pihole-FTL.service %{buildroot}%{_unitdir}
install -m 0644 aux/macvendor.db %{buildroot}/etc/pihole


%files
%license LICENSE
%{_bindir}/pihole-FTL
%{_unitdir}/pihole-FTL.service
/etc/pihole/macvendor.db


%post
# Only add the user when installing
if [ $1 -eq 1 ]; then
    # Create a pihole user and group if they don't already exist
    adduser --system --user-group pihole &>/dev/null
fi

# Give FTL permission to bind to low ports and have other advanced network
# capabilities
setcap CAP_NET_BIND_SERVICE,CAP_NET_RAW,CAP_NET_ADMIN+eip /usr/bin/pihole-FTL

# Set ownership of /etc/pihole
chown pihole:pihole -R /etc/pihole

%systemd_post pihole-FTL.service


%preun
%systemd_preun pihole-FTL.service


%postun
%systemd_postun_with_restart pihole-FTL.service


%changelog
* Sat Jun 08 2019 Mark Drobnak <mark.drobnak@pi-hole.net> - 4.3.1-1
- Initial package