#!/bin/sh
su rdiary -c "serve -s /usr/src/app/build -p 3000 --csp \"default-src 'self' ; object-src 'none'; script-src 'self' 'unsafe-inline' https://code.jquery.com; style-src 'self' https://fonts.googleapis.com 'unsafe-inline'; font-src 'self' https://fonts.gstatic.com; connect-src http://localhost:3001\""
