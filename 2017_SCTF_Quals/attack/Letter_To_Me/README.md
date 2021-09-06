# Letter to me

## Description

  > Letter to me

  > I have no friend.

  > so I made a service to send message to myself.T.T

  > [Link to the web service]

  > [Download link to schema.sql]

## exploit

* stage 0: source leak:

use LFI to leak source

* stage 1: be admin to register new user:

since you overwrite all value in conn.php, you can login as "admin".

when you become admin, you can make new user.

* stage 2: object injection:

As `profanity_word` is replaced (filtered) after object serialization, you can forge serialized data by overwriting `profanity_word_replace`.

send data to overwrite `attached_file`, and in consequence, execute a query that fetches flag.

