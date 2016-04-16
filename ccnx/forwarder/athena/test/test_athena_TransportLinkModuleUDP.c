/*
 * Copyright (c) 2015, Xerox Corporation (Xerox)and Palo Alto Research Center (PARC)
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Patent rights are not granted under this agreement. Patent rights are
 *       available under FRAND terms.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL XEROX or PARC BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
/**
 * @author Kevin Fox, Palo Alto Research Center (Xerox PARC)
 * @copyright 2015, Xerox Corporation (Xerox)and Palo Alto Research Center (PARC).  All rights reserved.
 */
#include <config.h>
#include "../athena_TransportLinkModuleUDP.c"
#include <LongBow/unit-test.h>

#include <parc/algol/parc_SafeMemory.h>
#include <parc/algol/parc_Network.h>
#include <ccnx/forwarder/athena/athena.h>
#include <ccnx/forwarder/athena/athena_TransportLinkAdapter.h>

LONGBOW_TEST_RUNNER(athena_TransportLinkModuleUDP)
{
    parcMemory_SetInterface(&PARCSafeMemoryAsPARCMemory);
    LONGBOW_RUN_TEST_FIXTURE(Global);
    LONGBOW_RUN_TEST_FIXTURE(Local);
}

LONGBOW_TEST_RUNNER_SETUP(athena_TransportLinkModuleUDP)
{
    return LONGBOW_STATUS_SUCCEEDED;
}

LONGBOW_TEST_RUNNER_TEARDOWN(athena_TransportLinkModuleUDP)
{
    return LONGBOW_STATUS_SUCCEEDED;
}

LONGBOW_TEST_FIXTURE(Global)
{
    LONGBOW_RUN_TEST_CASE(Global, athenaTransportLinkModuleUDP_OpenClose);
    LONGBOW_RUN_TEST_CASE(Global, athenaTransportLinkModuleUDP_SendReceive);
    LONGBOW_RUN_TEST_CASE(Global, athenaTransportLinkModuleUDP_MTU);
    LONGBOW_RUN_TEST_CASE(Global, athenaTransportLinkModuleUDP_P2P);
    LONGBOW_RUN_TEST_CASE(Global, athenaTransportLinkModuleUDP_Local);
}

LONGBOW_TEST_FIXTURE_SETUP(Global)
{
    return LONGBOW_STATUS_SUCCEEDED;
}

LONGBOW_TEST_FIXTURE_TEARDOWN(Global)
{
    uint32_t outstandingAllocations = parcSafeMemory_ReportAllocation(STDERR_FILENO);
    if (outstandingAllocations != 0) {
        printf("%s leaks memory by %d allocations\n", longBowTestCase_GetName(testCase), outstandingAllocations);
        return LONGBOW_STATUS_MEMORYLEAK;
    }
    return LONGBOW_STATUS_SUCCEEDED;
}

void
_removeLink(void *context, PARCBitVector *parcBitVector)
{
    assertNull(context, "_removeLink called with a non null argument");
    //AthenaTransportLinkAdapter *athenaTransportLinkAdapter = (AthenaTransportLinkAdapter *) context;
}

LONGBOW_TEST_CASE(Global, athenaTransportLinkModuleUDP_OpenClose)
{
    PARCURI *connectionURI;
    const char *result;
    AthenaTransportLinkAdapter *athenaTransportLinkAdapter = athenaTransportLinkAdapter_Create(_removeLink, NULL);
    assertNotNull(athenaTransportLinkAdapter, "athenaTransportLinkAdapter_Create returned NULL");

    connectionURI = parcURI_Parse("udp://127.0.0.1:40000/name=");
    result = athenaTransportLinkAdapter_Open(athenaTransportLinkAdapter, connectionURI);
    assertTrue(result == NULL, "athenaTransportLinkAdapter_Open failed to detect bad name argument");
    parcURI_Release(&connectionURI);

    connectionURI = parcURI_Parse("udp://127.0.0.1:40000/local=");
    result = athenaTransportLinkAdapter_Open(athenaTransportLinkAdapter, connectionURI);
    assertTrue(result == NULL, "athenaTransportLinkAdapter_Open failed to detect bad local argument");
    parcURI_Release(&connectionURI);

    connectionURI = parcURI_Parse("udp://127.0.0.1:40000/src=");
    result = athenaTransportLinkAdapter_Open(athenaTransportLinkAdapter, connectionURI);
    assertTrue(result == NULL, "athenaTransportLinkAdapter_Open failed to detect bad source argument");
    parcURI_Release(&connectionURI);

    connectionURI = parcURI_Parse("udp://127.0.0.1/name=UDP_1");
    result = athenaTransportLinkAdapter_Open(athenaTransportLinkAdapter, connectionURI);
    assertTrue(result == NULL, "athenaTransportLinkAdapter_Open failed to detect bad address specification");
    parcURI_Release(&connectionURI);

    connectionURI = parcURI_Parse("udp://127.0.0.1:40000/Listene/name=UDP_1");
    result = athenaTransportLinkAdapter_Open(athenaTransportLinkAdapter, connectionURI);
    assertTrue(result == NULL, "athenaTransportLinkAdapter_Open failed to detect bad argument");
    parcURI_Release(&connectionURI);

    connectionURI = parcURI_Parse("udp://127.0.0.1:40000/Listener/nameo=");
    result = athenaTransportLinkAdapter_Open(athenaTransportLinkAdapter, connectionURI);
    assertTrue(result == NULL, "athenaTransportLinkAdapter_Open failed to detect bad name specification");
    parcURI_Release(&connectionURI);

    connectionURI = parcURI_Parse("udp://127.0.0.1:40000/Listener/name=");
    result = athenaTransportLinkAdapter_Open(athenaTransportLinkAdapter, connectionURI);
    assertTrue(result == NULL, "athenaTransportLinkAdapter_Open failed to detect bad name specification");
    parcURI_Release(&connectionURI);

    connectionURI = parcURI_Parse("udp://127.0.0.1:40000/Listener/name=UDP_1");
    result = athenaTransportLinkAdapter_Open(athenaTransportLinkAdapter, connectionURI);
    assertTrue(result != NULL, "athenaTransportLinkAdapter_Open failed (%s)", strerror(errno));
    parcURI_Release(&connectionURI);

    connectionURI = parcURI_Parse("udp://127.0.0.1:40000/Listener/name=UDP_1");
    result = athenaTransportLinkAdapter_Open(athenaTransportLinkAdapter, connectionURI);
    assertTrue(result == NULL, "athenaTransportLinkAdapter_Open succeeded in opening a duplicate link");
    parcURI_Release(&connectionURI);

    int closeResult = athenaTransportLinkAdapter_CloseByName(athenaTransportLinkAdapter, "UDP_1");
    assertTrue(closeResult == 0, "athenaTransportLinkAdapter_CloseByName failed (%s)", strerror(errno));

    athenaTransportLinkAdapter_Destroy(&athenaTransportLinkAdapter);
}

LONGBOW_TEST_CASE(Global, athenaTransportLinkModuleUDP_SendReceive)
{
    PARCURI *connectionURI;
    const char *result;
    AthenaTransportLinkAdapter *athenaTransportLinkAdapter = athenaTransportLinkAdapter_Create(_removeLink, NULL);
    assertNotNull(athenaTransportLinkAdapter, "athenaTransportLinkAdapter_Create returned NULL");

    connectionURI = parcURI_Parse("udp://127.0.0.1:40000/Listener/name=UDPListener");
    result = athenaTransportLinkAdapter_Open(athenaTransportLinkAdapter, connectionURI);
    assertTrue(result != NULL, "athenaTransportLinkAdapter_Open failed (%s)", strerror(errno));
    parcURI_Release(&connectionURI);

    connectionURI = parcURI_Parse("udp://127.0.0.1:40000/name=UDP_1");
    result = athenaTransportLinkAdapter_Open(athenaTransportLinkAdapter, connectionURI);
    assertTrue(result != NULL, "athenaTransportLinkAdapter_Open failed (%s)", strerror(errno));
    parcURI_Release(&connectionURI);

    athenaTransportLinkAdapter_Poll(athenaTransportLinkAdapter, 0);

    CCNxName *name = ccnxName_CreateFromCString("lci:/foo/bar");
    CCNxMetaMessage *ccnxMetaMessage = ccnxInterest_CreateSimple(name);
    ccnxName_Release(&name);

    PARCBitVector *sendVector = parcBitVector_Create();

    int linkId = athenaTransportLinkAdapter_LinkNameToId(athenaTransportLinkAdapter, "UDP_1");
    parcBitVector_Set(sendVector, linkId);

    athena_EncodeMessage(ccnxMetaMessage);
    PARCBitVector *resultVector;
    resultVector = athenaTransportLinkAdapter_Send(athenaTransportLinkAdapter, ccnxMetaMessage, sendVector);
    assertNotNull(resultVector, "athenaTransportLinkAdapter_Send failed");
    parcBitVector_Release(&resultVector);
    ccnxMetaMessage_Release(&ccnxMetaMessage);
    parcBitVector_Release(&sendVector);

    usleep(1000);

    ccnxMetaMessage = athenaTransportLinkAdapter_Receive(athenaTransportLinkAdapter, &resultVector, 0);
    assertNotNull(resultVector, "athenaTransportLinkAdapter_Receive failed");
    assertTrue(parcBitVector_NumberOfBitsSet(resultVector) == 1, "athenaTransportLinkAdapter_Receive return message with more than one ingress link");
    assertNotNull(ccnxMetaMessage, "athenaTransportLinkAdapter_Receive failed to provide message");
    parcBitVector_Release(&resultVector);
    ccnxMetaMessage_Release(&ccnxMetaMessage);

    // Close one end of the connection and send a message from the other.
    int closeResult = athenaTransportLinkAdapter_CloseByName(athenaTransportLinkAdapter, "UDPListener");
    assertTrue(closeResult == 0, "athenaTransportLinkAdapter_CloseByName failed (%s)", strerror(errno));

    ccnxMetaMessage = athenaTransportLinkAdapter_Receive(athenaTransportLinkAdapter, &resultVector, 1);
    assertNull(resultVector, "athenaTransportLinkAdapter_Receive should have failed");

    closeResult = athenaTransportLinkAdapter_CloseByName(athenaTransportLinkAdapter, "UDP_1");
    assertTrue(closeResult == 0, "athenaTransportLinkAdapter_CloseByName failed (%s)", strerror(errno));

    athenaTransportLinkAdapter_Destroy(&athenaTransportLinkAdapter);
}

LONGBOW_TEST_CASE(Global, athenaTransportLinkModuleUDP_MTU)
{
    PARCURI *connectionURI;
    const char *result;
    AthenaTransportLinkAdapter *athenaTransportLinkAdapter = athenaTransportLinkAdapter_Create(_removeLink, NULL);
    assertNotNull(athenaTransportLinkAdapter, "athenaTransportLinkAdapter_Create returned NULL");

    connectionURI = parcURI_Parse("udp://127.0.0.1:40000/Listener/name=UDPListener/mtu=");
    result = athenaTransportLinkAdapter_Open(athenaTransportLinkAdapter, connectionURI);
    assertTrue(result == NULL, "athenaTransportLinkAdapter_Open failed to detect improper MTU (%s)", strerror(errno));
    parcURI_Release(&connectionURI);

    connectionURI = parcURI_Parse("udp://127.0.0.1:40000/Listener/name=UDPListener/mtu=20");
    result = athenaTransportLinkAdapter_Open(athenaTransportLinkAdapter, connectionURI);
    assertTrue(result != NULL, "athenaTransportLinkAdapter_Open failed (%s)", strerror(errno));
    parcURI_Release(&connectionURI);

    connectionURI = parcURI_Parse("udp://127.0.0.1:40000/name=UDP_1/mtu=20");
    result = athenaTransportLinkAdapter_Open(athenaTransportLinkAdapter, connectionURI);
    assertTrue(result != NULL, "athenaTransportLinkAdapter_Open failed (%s)", strerror(errno));
    parcURI_Release(&connectionURI);

    athenaTransportLinkAdapter_Poll(athenaTransportLinkAdapter, 0);

    CCNxName *name = ccnxName_CreateFromCString("lci:/foo/bar");
    CCNxMetaMessage *ccnxMetaMessage = ccnxInterest_CreateSimple(name);
    ccnxName_Release(&name);

    PARCBitVector *sendVector = parcBitVector_Create();

    int linkId = athenaTransportLinkAdapter_LinkNameToId(athenaTransportLinkAdapter, "UDP_1");
    parcBitVector_Set(sendVector, linkId);

    athena_EncodeMessage(ccnxMetaMessage);
    PARCBitVector *resultVector;
    resultVector = athenaTransportLinkAdapter_Send(athenaTransportLinkAdapter, ccnxMetaMessage, sendVector);
    assertTrue(parcBitVector_NumberOfBitsSet(resultVector) == 0, "athenaTransportLinkAdapter_Send should have failed");
    assertTrue(errno == EMSGSIZE, "athenaTransportLinkAdapter_Send should have failed with EMSGSIZE (%d): %s", errno, strerror(errno));
    parcBitVector_Release(&resultVector);
    ccnxMetaMessage_Release(&ccnxMetaMessage);
    parcBitVector_Release(&sendVector);

    usleep(1000);

    ccnxMetaMessage = athenaTransportLinkAdapter_Receive(athenaTransportLinkAdapter, &resultVector, 0);
    assertNull(ccnxMetaMessage, "athenaTransportLinkAdapter_Receive should have failed");

    int closeResult = athenaTransportLinkAdapter_CloseByName(athenaTransportLinkAdapter, "UDPListener");
    assertTrue(closeResult == 0, "athenaTransportLinkAdapter_CloseByName failed (%s)", strerror(errno));

    closeResult = athenaTransportLinkAdapter_CloseByName(athenaTransportLinkAdapter, "UDP_1");
    assertTrue(closeResult == 0, "athenaTransportLinkAdapter_CloseByName failed (%s)", strerror(errno));

    athenaTransportLinkAdapter_Destroy(&athenaTransportLinkAdapter);
}

LONGBOW_TEST_CASE(Global, athenaTransportLinkModuleUDP_P2P)
{
    PARCURI *connectionURI;
    const char *result;
    AthenaTransportLinkAdapter *athenaTransportLinkAdapter = athenaTransportLinkAdapter_Create(_removeLink, NULL);
    assertNotNull(athenaTransportLinkAdapter, "athenaTransportLinkAdapter_Create returned NULL");

    connectionURI = parcURI_Parse("udp://localhost:40001/src=localhost:40002/name=UDP_0");
    result = athenaTransportLinkAdapter_Open(athenaTransportLinkAdapter, connectionURI);
    assertTrue(result != NULL, "athenaTransportLinkAdapter_Open failed (%s)", strerror(errno));
    parcURI_Release(&connectionURI);

    connectionURI = parcURI_Parse("udp://localhost:40002/src=localhost:40001/name=UDP_1");
    result = athenaTransportLinkAdapter_Open(athenaTransportLinkAdapter, connectionURI);
    assertTrue(result != NULL, "athenaTransportLinkAdapter_Open failed (%s)", strerror(errno));
    parcURI_Release(&connectionURI);

    CCNxName *name = ccnxName_CreateFromCString("lci:/foo/bar");
    CCNxMetaMessage *ccnxMetaMessage = ccnxInterest_CreateSimple(name);
    ccnxName_Release(&name);

    PARCBitVector *sendVector = parcBitVector_Create();

    int linkId = athenaTransportLinkAdapter_LinkNameToId(athenaTransportLinkAdapter, "UDP_1");
    parcBitVector_Set(sendVector, linkId);

    athena_EncodeMessage(ccnxMetaMessage);
    PARCBitVector *resultVector;
    resultVector = athenaTransportLinkAdapter_Send(athenaTransportLinkAdapter, ccnxMetaMessage, sendVector);
    assertNotNull(resultVector, "athenaTransportLinkAdapter_Send failed");
    parcBitVector_Release(&resultVector);
    ccnxMetaMessage_Release(&ccnxMetaMessage);
    parcBitVector_Release(&sendVector);

    usleep(1000);

    ccnxMetaMessage = athenaTransportLinkAdapter_Receive(athenaTransportLinkAdapter, &resultVector, 0);
    assertNotNull(resultVector, "athenaTransportLinkAdapter_Receive failed");
    assertTrue(parcBitVector_NumberOfBitsSet(resultVector) == 1, "athenaTransportLinkAdapter_Receive return message with more than one ingress link");
    assertNotNull(ccnxMetaMessage, "athenaTransportLinkAdapter_Receive failed to provide message");
    parcBitVector_Release(&resultVector);
    ccnxMetaMessage_Release(&ccnxMetaMessage);

    athenaTransportLinkAdapter_Destroy(&athenaTransportLinkAdapter);
}

LONGBOW_TEST_CASE(Global, athenaTransportLinkModuleUDP_Local)
{
    PARCURI *connectionURI;
    const char *result;
    AthenaTransportLinkAdapter *athenaTransportLinkAdapter = athenaTransportLinkAdapter_Create(_removeLink, NULL);
    assertNotNull(athenaTransportLinkAdapter, "athenaTransportLinkAdapter_Create returned NULL");

    connectionURI = parcURI_Parse("udp://127.0.0.1:40000/Listener/name=UDP_0");
    result = athenaTransportLinkAdapter_Open(athenaTransportLinkAdapter, connectionURI);
    assertTrue(result != NULL, "athenaTransportLinkAdapter_Open failed (%s)", strerror(errno));
    parcURI_Release(&connectionURI);

    connectionURI = parcURI_Parse("udp://127.0.0.1:40000/name=UDP_1/local=boo");
    result = athenaTransportLinkAdapter_Open(athenaTransportLinkAdapter, connectionURI);
    assertTrue(result == NULL, "athenaTransportLinkAdapter_Open failed to detect bad local directive");
    parcURI_Release(&connectionURI);

    connectionURI = parcURI_Parse("udp://127.0.0.1:40000/name=UDP_1/local=true");
    result = athenaTransportLinkAdapter_Open(athenaTransportLinkAdapter, connectionURI);
    assertTrue(result != NULL, "athenaTransportLinkAdapter_Open failed (%s)", strerror(errno));
    parcURI_Release(&connectionURI);

    connectionURI = parcURI_Parse("udp://127.0.0.1:40000/name=UDP_2/local=false");
    result = athenaTransportLinkAdapter_Open(athenaTransportLinkAdapter, connectionURI);
    assertTrue(result != NULL, "athenaTransportLinkAdapter_Open failed (%s)", strerror(errno));
    parcURI_Release(&connectionURI);

    int linkId = athenaTransportLinkAdapter_LinkNameToId(athenaTransportLinkAdapter, "UDP_1");
    assertFalse(athenaTransportLinkAdapter_IsNotLocal(athenaTransportLinkAdapter, linkId), "Local connection not local");

    linkId = athenaTransportLinkAdapter_LinkNameToId(athenaTransportLinkAdapter, "UDP_2");
    assertTrue(athenaTransportLinkAdapter_IsNotLocal(athenaTransportLinkAdapter, linkId), "NonLocal connection is local");

    int closeResult = athenaTransportLinkAdapter_CloseByName(athenaTransportLinkAdapter, "UDP_2");
    assertTrue(closeResult == 0, "athenaTransportLinkAdapter_CloseByName failed (%s)", strerror(errno));

    closeResult = athenaTransportLinkAdapter_CloseByName(athenaTransportLinkAdapter, "UDP_1");
    assertTrue(closeResult == 0, "athenaTransportLinkAdapter_CloseByName failed (%s)", strerror(errno));

    closeResult = athenaTransportLinkAdapter_CloseByName(athenaTransportLinkAdapter, "UDP_0");
    assertTrue(closeResult == 0, "athenaTransportLinkAdapter_CloseByName failed (%s)", strerror(errno));

    athenaTransportLinkAdapter_Destroy(&athenaTransportLinkAdapter);
}

LONGBOW_TEST_FIXTURE(Local)
{
}

LONGBOW_TEST_FIXTURE_SETUP(Local)
{
    return LONGBOW_STATUS_SUCCEEDED;
}

LONGBOW_TEST_FIXTURE_TEARDOWN(Local)
{
    uint32_t outstandingAllocations = parcSafeMemory_ReportAllocation(STDERR_FILENO);
    if (outstandingAllocations != 0) {
        printf("%s leaks memory by %d allocations\n", longBowTestCase_GetName(testCase), outstandingAllocations);
        return LONGBOW_STATUS_MEMORYLEAK;
    }
    return LONGBOW_STATUS_SUCCEEDED;
}

int
main(int argc, char *argv[])
{
    LongBowRunner *testRunner = LONGBOW_TEST_RUNNER_CREATE(athena_TransportLinkModuleUDP);
    exit(longBowMain(argc, argv, testRunner, NULL));
}
