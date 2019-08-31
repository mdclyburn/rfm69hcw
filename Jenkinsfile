// -*- mode: Groovy; groovy-indent-offset: 2 -*-

pipeline {
  agent { node { label 'master' } }

  stages {
    stage('Documentation') {
      steps {
        sh 'doxygen >/dev/null'
        dir ('doc') {
          dir ('latex') {
            sh  'latexmk -pdf -dvi- pquiet refman.tex'
            stash(name: 'docs-pdf', includes: 'refman.pdf')
          }

          sh 'tar -cf web.tar -C html .'
          stash(name: 'docs-web', includes: 'web.tar')
        }
      }

      when {
        branch 'master';
      }

      post {
        always {
          sh 'rm -rf docs'
        }
      }
    }

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

    stage('Publish') {
      agent { node { label 'www' } }
      steps {
        script {
          DEPLOY_DIR = '/usr/local/www/build_exports/mdl-rfm69hcw'
        }

        dir ("${DEPLOY_DIR}/docs") {
          sh 'rm -rf *'

          unstash(name: 'docs-pdf')

          dir ('web') {
            unstash(name: 'docs-web')
            sh 'tar -xf web.tar'
            sh 'rm -f web.tar'
          }
        }
      }
    }
  }
}
