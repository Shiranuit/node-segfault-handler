FROM node:12.18.1-alpine3.11

RUN  set -x \
  && apk add --no-cache --no-progress --virtual .gyp \
      python \
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