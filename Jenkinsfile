pipeline {
  agent { node { label 'master' } }

  stages {
    stage ('Build') {
      agent { node { label 'msp430' } }

      steps {
        dir ('src') {
          sh 'git submodule init && git submodule update'
          sh 'make MSP430PATH=/usr/local/msp430 MSP430MCU=msp430g2553'
        }
      }

      post {
        always {
          dir ('src') {
            sh 'make clean'
          }
        }
      }
    }
  }
}
