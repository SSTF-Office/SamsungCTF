lazy val root = (project in file(".")).
  settings(
      name := "simpleDSL",
      version := "1.0",
      scalaVersion := "2.11.6",
      libraryDependencies += "org.scala-lang.modules" %% "scala-parser-combinators" % "1.1.0",
      libraryDependencies += "com.github.scopt" %% "scopt" % "3.7.0",
      libraryDependencies += "org.scalatest" %% "scalatest" % "3.0.5" % "test",
      scalacOptions ++= Seq("-feature")
      )
