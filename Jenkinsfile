pipeline {
  agent any
  
  stages {
    stage('Build') {
      steps {
        sh 'make'
      }
    }

    stage('Test') {
      steps {
        sh 'make tests'
      }
    }

    stage('Test2') {
      steps {
        sh 'make tests2'
      }
    }
  }
}
