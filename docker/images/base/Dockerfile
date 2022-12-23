ARG NODE_VERSION=12.16.3
FROM node:${NODE_VERSION}

RUN  set -x \
  && apt update \
  && apt install -y gyp \
      python3 \
      make \
      g++ \
      libunwind-dev

ADD . /var/app
ADD ./docker/scripts/entrypoint.sh /bin/entrypoint

WORKDIR /var/app

RUN npm install --unsafe-perm
RUN chmod +x /bin/entrypoint

ENTRYPOINT [ "/bin/entrypoint" ]
CMD ["node", "tests/test.js"]