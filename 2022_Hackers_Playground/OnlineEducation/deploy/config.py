# should be secret
secret_key = "19eb794c831f30f099a31b1c095a17d6"
admin_hash = "19eb794c831f30f099a31b1c095a17d6"

# sample data
course_data = {
    'name': 'Sample Education',
    'author': 'SSTF',
    'vids': [
        {'name': 'Course 1',
         'url': '/static/vids/min_ed1081dfc91fdccefe60094faa633abc.webm',
         'type': 'video/webm',
         'thumbnail': '/static/vids/min.png',
         'length': 60*2,
         'desc': '2 mins'},
        {'name': 'Course 2',
         'url': '/static/vids/moment_61bfe721895bddf955f30f6ec08e165f.webm',
         'type': 'video/webm',
         'thumbnail': '/static/vids/moment.png',
         'length': 60*15,
         'desc': '15 mins'},
        {'name': 'Course Final',
         'url': '/static/vids/many_9fa0b0f83487974654530648c79590e2.webm',
         'type': 'video/webm',
         'thumbnail': '/static/vids/many.png',
         'length': 60*60*25+60*15,
         'desc': '? mins'},
    ],
}



