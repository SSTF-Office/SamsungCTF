--
-- PostgreSQL database cluster dump
--

-- Started on 2023-01-19 17:32:29 KST

SET default_transaction_read_only = off;

SET client_encoding = 'UTF8';
SET standard_conforming_strings = on;

--
-- Roles
--

CREATE ROLE books;
ALTER ROLE books WITH SUPERUSER INHERIT CREATEROLE LOGIN REPLICATION BYPASSRLS PASSWORD 'SCRAM-SHA-256$4096:7yNOGWNRrzN4ZJDFA90v0g==$tG9i9Cvl81aB4af0il6hTBPCJp23HehBqH+7gDV8C5c=:phtp345hppnUeDjtWrhlAL97p306THGZSoycCdBnBrg=';


--
-- Databases
--

--
-- Database "template1" dump
--

\connect template1

--
-- PostgreSQL database dump
--

-- Dumped from database version 14.6
-- Dumped by pg_dump version 14.6 (Ubuntu 14.6-0ubuntu0.22.04.1)

-- Started on 2023-01-19 17:32:29 KST

SET statement_timeout = 0;
SET lock_timeout = 0;
SET idle_in_transaction_session_timeout = 0;
SET client_encoding = 'UTF8';
SET standard_conforming_strings = on;
SELECT pg_catalog.set_config('search_path', '', false);
SET check_function_bodies = false;
SET xmloption = content;
SET client_min_messages = warning;
SET row_security = off;

-- Completed on 2023-01-19 17:32:29 KST

--
-- PostgreSQL database dump complete
--

--
-- Database "books" dump
--

--
-- PostgreSQL database dump
--

-- Dumped from database version 14.6
-- Dumped by pg_dump version 14.6 (Ubuntu 14.6-0ubuntu0.22.04.1)

-- Started on 2023-01-19 17:32:29 KST

SET statement_timeout = 0;
SET lock_timeout = 0;
SET idle_in_transaction_session_timeout = 0;
SET client_encoding = 'UTF8';
SET standard_conforming_strings = on;
SELECT pg_catalog.set_config('search_path', '', false);
SET check_function_bodies = false;
SET xmloption = content;
SET client_min_messages = warning;
SET row_security = off;

--
-- TOC entry 3348 (class 1262 OID 16385)
-- Name: books; Type: DATABASE; Schema: -; Owner: postgres
--

CREATE DATABASE books WITH TEMPLATE = template0 ENCODING = 'UTF8' LOCALE = 'en_US.utf8';


ALTER DATABASE books OWNER TO books;

\connect books

SET statement_timeout = 3000;
SET lock_timeout = 0;
SET idle_in_transaction_session_timeout = 0;
SET client_encoding = 'UTF8';
SET standard_conforming_strings = on;
SELECT pg_catalog.set_config('search_path', '', false);
SET check_function_bodies = false;
SET xmloption = content;
SET client_min_messages = warning;
SET row_security = off;

--
-- TOC entry 5 (class 2615 OID 16386)
-- Name: books; Type: SCHEMA; Schema: -; Owner: postgres
--

CREATE SCHEMA books;


ALTER SCHEMA books OWNER TO books;

--
-- TOC entry 3349 (class 0 OID 0)
-- Dependencies: 5
-- Name: SCHEMA books; Type: COMMENT; Schema: -; Owner: postgres
--

COMMENT ON SCHEMA books IS 'standard public schema';


SET default_tablespace = '';

SET default_table_access_method = heap;

--
-- TOC entry 3350 (class 0 OID 0)
-- Dependencies: 5
-- Name: SCHEMA books; Type: ACL; Schema: -; Owner: postgres
--

GRANT USAGE ON SCHEMA books TO PUBLIC;


-- Completed on 2023-01-19 17:32:30 KST

--
-- PostgreSQL database dump complete
--

--
-- Database "postgres" dump
--

\connect postgres

--
-- PostgreSQL database dump
--

-- Dumped from database version 14.6
-- Dumped by pg_dump version 14.6 (Ubuntu 14.6-0ubuntu0.22.04.1)

-- Started on 2023-01-19 17:32:30 KST

SET statement_timeout = 0;
SET lock_timeout = 0;
SET idle_in_transaction_session_timeout = 0;
SET client_encoding = 'UTF8';
SET standard_conforming_strings = on;
SELECT pg_catalog.set_config('search_path', '', false);
SET check_function_bodies = false;
SET xmloption = content;
SET client_min_messages = warning;
SET row_security = off;

-- Completed on 2023-01-19 17:32:30 KST

--
-- PostgreSQL database dump complete
--

-- Completed on 2023-01-19 17:32:30 KST

--
-- PostgreSQL database cluster dump complete
--
