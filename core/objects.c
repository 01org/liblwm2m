/*
Copyright (c) 2013, Intel Corporation

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

    * Redistributions of source code must retain the above copyright notice,
      this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright notice,
      this list of conditions and the following disclaimer in the documentation
      and/or other materials provided with the distribution.
    * Neither the name of Intel Corporation nor the names of its contributors
      may be used to endorse or promote products derived from this software
      without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
THE POSSIBILITY OF SUCH DAMAGE.

David Navarro <david.navarro@intel.com>

*/

#include "internals.h"

#include <stdlib.h>
#include <string.h>


static lwm2m_object_t * prv_find_object(lwm2m_context_t * contextP,
                                        uint16_t Id)
{
    int i;

    i = 0;
    while (i < contextP->numObject && contextP->objectList[i]->objID < Id)
    {
        i++;
    }
    if (i == contextP->numObject)
    {
        return NULL;
    }
    else
    {
        return contextP->objectList[i];
    }
}

coap_status_t object_read(lwm2m_context_t * contextP,
                          lwm2m_uri_t * uriP,
                          char ** bufferP,
                          int * lengthP)
{
    lwm2m_object_t * targetP;

    targetP = prv_find_object(contextP, uriP->objID);

    if (NULL == targetP)
    {
        return NOT_FOUND_4_04;
    }
    if (NULL == targetP->readFunc)
    {
        return METHOD_NOT_ALLOWED_4_05;
    }

    return targetP->readFunc(uriP, bufferP, lengthP, targetP);
}


coap_status_t object_write(lwm2m_context_t * contextP,
                           lwm2m_uri_t * uriP,
                           char * buffer,
                           int length)
{
    lwm2m_object_t * targetP;

    targetP = prv_find_object(contextP, uriP->objID);

    if (NULL == targetP)
    {
        return NOT_FOUND_4_04;
    }
    if (NULL == targetP->writeFunc)
    {
        return METHOD_NOT_ALLOWED_4_05;
    }

    return targetP->writeFunc(uriP, buffer, length, targetP);
}

